// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CGame.h"
#include "CStage.h"
#include "CTexture.h"
#include "CFont.h"
#include "CRenderer.h"
#include "CArtifact.h"
#include "CGameBoard.h"
#include "CInterpolator.h"
#include "CAnimatedSprite.h"
#include "CTextControl.h"
#include "SDL_mixer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	These should go into a config file

static const int sGridColCount = 8;
static const int sGridRowCount = 8;
static const int sCellWidth = 42;
static const int sCellHeight = 44;
static const int sGridLeft = 332;
static const int sGridTop = 100;

// timings
#if _DEBUG
static const Uint32 sGameDuration = 4 * 1000;
#else
static const Uint32 sGameDuration = 60 * 1000;
#endif
static const Uint32 sDropDuration = 100;
static const Uint32 sSwapDuration = 200;

static std::vector<SDL_Point> fuseCoords ({ { 246, 538 }, { 235, 528 }, { 236, 513 }, { 236, 498 }, { 220, 492 }, { 198, 481 }, { 195, 466 }, { 195, 445 }, { 193, 416 }, { 191, 385 }, { 189, 356 }, { 172, 355 }, { 158, 355 } } );


////////////////////////////////////////////////////////////////////////////////////////////////////


CGame::CGame (CStagePtr inStage, const std::string inAssetsPath) :
	mStage (inStage),
	mAssetsPath (inAssetsPath),
	mTypeCount (0),
	mDisableInputCount (0),
	mHasSelection (false),
	mIsSwapping (false),
	mSwapEndTicks (0),
	mIsDropping (false),
	mDropEndTicks (0),
	mIsExploding (false),
	mClipRect ({ sGridLeft, sGridTop, sGridColCount * sCellWidth, sGridRowCount * sCellHeight }),
	mGameRunning (false),
	mGameFinished (false),
	mGameEndTicks (0),
	mSoundFxFuse (0),
	mSoundFxExplosion (0),
	mFuseChannel (-1),
	mScore (0)
{
	mStage->SetBackground (mAssetsPath + "BackGround.jpg");

	CreateTextures();
	CreateObjects();

	LoadSoundFx();
}


CGame::~CGame (void)
{
	mArtifacts.Clear();
	mGameBoard = nullptr;

	for (unsigned int i = 0; i < mSoundFxBreak.size(); i++)
		Mix_FreeChunk (mSoundFxBreak[i]);
	mSoundFxBreak.clear();

	Mix_FreeChunk (mSoundFxFuse);
	mSoundFxFuse = 0;

	Mix_FreeChunk (mSoundFxExplosion);
	mSoundFxExplosion = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::CreateTextures (void)
{
	const char *fileNames[] = { "Blue.png", "Green.png", "Purple.png", "Red.png", "Yellow.png" };

	mTypeCount = sizeof (fileNames) / sizeof (*fileNames);

	mArtifactTextures.resize (mTypeCount);

	for (int i = 0; i < mTypeCount; i++)
		mArtifactTextures[i] = mStage->CreateTexture (mAssetsPath + fileNames[i]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::CreateObjects (void)
{
	mGameBoard = std::make_shared<CGameBoard> (sGridColCount, sGridRowCount, mTypeCount);
	mGameBoard->Clear();

	// create fuse sparks sprite
	std::vector<std::string> fuseFileNames ({ mAssetsPath + "fuse1.png", mAssetsPath + "fuse2.png", mAssetsPath + "fuse3.png" });
	mFuse = std::make_shared<CAnimatedSprite> (mStage, fuseFileNames);
	mFuse->SetFps (15);

	SDL_Color alertTextColor = { 255, 255, 255 };
	mAlertFont = std::make_shared<CFont> (mAssetsPath + "Fonts\\freefont\\FreeSans.ttf", 18);
	mAlertTextControl = std::make_shared<CTextControl> (mStage, 80, 120, mAlertFont, alertTextColor);

	SDL_Color scoreTextColor = { 0, 0, 255 };
	mScoreFont = std::make_shared<CFont> (mAssetsPath + "Fonts\\freefont\\FreeSans.ttf", 24);
	mScoreTextControl = std::make_shared<CTextControl> (mStage, 80, 200, mScoreFont, scoreTextColor);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::ShowAlert (std::string inText, SDL_Point *inPos)
{
	// makes sure it's not already shown
	mStage->RemoveDrawable (mAlertTextControl);

	mAlertTextControl->SetText (inText);

	if (inPos)
		mAlertTextControl->SetPosition (*inPos);

	mStage->AddDrawable (mAlertTextControl, 2); // add above normal drawables
}


void
CGame::HideAlert (void)
{
	mStage->RemoveDrawable (mAlertTextControl);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::LoadSoundFx (void)
{
	const char *fileNames[] = { "break1.wav", "break2.wav", "break3.wav" };

	unsigned int count = sizeof (fileNames) / sizeof (*fileNames);

	mSoundFxBreak.resize (count);

	for (unsigned int i = 0; i < count; i++)
		mSoundFxBreak[i] = Mix_LoadWAV ((mAssetsPath + "Sounds\\" + fileNames[i]).c_str());

	mSoundFxFuse = Mix_LoadWAV ((mAssetsPath + "Sounds\\fuse.wav").c_str());
	mSoundFxExplosion = Mix_LoadWAV ((mAssetsPath + "Sounds\\explosion.wav").c_str());
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::StartFuse (Uint32 inCurTicks)
{
	mFuse->SetPath (fuseCoords, inCurTicks, mGameEndTicks);
	mStage->AddDrawable (mFuse);

	// start fuse sound (looping)
	if (mSoundFxFuse)
		mFuseChannel = Mix_PlayChannel (-1, mSoundFxFuse, -1);
}


void
CGame::StopFuse (void)
{
	mStage->RemoveDrawable (mFuse);

	// stop fuse sound
	if (mFuseChannel >= 0)
		Mix_HaltChannel (mFuseChannel);
	mFuseChannel = -1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::Prepare (void)
{
	mScore = 0;

	mGameBoard->Fill();

	mArtifacts.Resize (sGridColCount, sGridRowCount);

	for (int x = 0; x < sGridColCount; x++)
	{
		for (int y = 0; y < sGridRowCount; y++)
		{
			int type = mGameBoard->Get (x,y);
			SDL_Point thePos = GetCellPosition ({ x, y });

			CArtifactPtr artifact = std::make_shared<CArtifact> (mStage, mArtifactTextures[type]);

			artifact->SetPosition (thePos);
			artifact->SetClip (&mClipRect);

			mArtifacts.Set (x, y, artifact);
			mStage->AddDrawable (artifact);
		}
	}

	if (mGameBoard->RemoveCombinations (mRemovedEntries))
		StartExplode();

	StartDropping();

	ShowAlert ("Press N to start");
}


void
CGame::Start (void)
{
	Uint32 curTicks = SDL_GetTicks();
	mGameEndTicks = curTicks + sGameDuration;
	mGameRunning = true;
	mGameFinished = false;

	StartFuse (curTicks);

	HideAlert();

	// show score (which is hidden the first time around)
	mStage->RemoveDrawable (mScoreTextControl);
	mScoreTextControl->SetText ("Score: 0");
	mStage->AddDrawable (mScoreTextControl);
}


void
CGame::Finish (void)
{
	StopFuse();

	if (mSoundFxExplosion)
		Mix_PlayChannel (-1, mSoundFxExplosion, 0);

	mGameRunning = false;
	mGameFinished = true;
	mGameEndTicks = 0;

	mRemovedEntries.clear();
	for (int x = 0; x < sGridColCount; x++)
		for (int y = 0; y < sGridRowCount; y++)
			mRemovedEntries.push_back ({ x, y });

	StartExplode();

	// wait a fraction of a second to sync the sound better
	SDL_Delay (200);

	SDL_Point newPos = { 400, 200 };
	ShowAlert ("Press N to play again", &newPos);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::RemoveArtifact (const SDL_Point &inPt)
{
	mStage->RemoveDrawable (mArtifacts.Get(inPt));
	mArtifacts.Set (inPt, nullptr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


SDL_Point
CGame::GetCellPosition (const SDL_Point &inPt)
{
	return { sGridLeft + inPt.x * sCellWidth, sGridTop + inPt.y * sCellHeight };
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::StartExplode (void)
{
	// TO DO: implement some nice explosions
	// note: explosions should not disable the input

	mIsExploding = true;
	if (mSoundFxBreak.size() > 0)
		Mix_PlayChannel (-1, mSoundFxBreak[rand() % mSoundFxBreak.size()], 0);

	std::for_each (mRemovedEntries.begin (), mRemovedEntries.end (), [this] (SDL_Point inPt)
	{
		RemoveArtifact (inPt);
	});
	mRemovedEntries.clear();
}


void
CGame::UpdateExplode (Uint32 inCurTicks)
{
	if (!mIsExploding)
		return;

	// TO DO: implement some nice explosions

	mIsExploding = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::StartSwap (SDL_Point inPt1, SDL_Point inPt2, bool inCanceling)
{
	mDisableInputCount++;
	mIsSwapping = true;

	mIsCanceling = inCanceling;

	const Uint32 ticks = SDL_GetTicks();
	mSwapEndTicks = ticks + sSwapDuration;

	mArtifact1 = mArtifacts.Get (mSelectedCell1);
	mArtifact2 = mArtifacts.Get (mSelectedCell2);

	if (mArtifact1) mArtifact1->StartMoveTo (GetCellPosition(mSelectedCell2), ticks, mSwapEndTicks);
	if (mArtifact2) mArtifact2->StartMoveTo (GetCellPosition(mSelectedCell1), ticks, mSwapEndTicks);
}


void
CGame::UpdateSwap (Uint32 inCurTicks)
{
	if (!mIsSwapping)
		return;

	if (inCurTicks > mSwapEndTicks)
	{
		if (mArtifact1) mArtifact1->FinishMoveNow();
		if (mArtifact2) mArtifact2->FinishMoveNow();

		mArtifacts.Set (mSelectedCell1, mArtifact2);
		mArtifacts.Set (mSelectedCell2, mArtifact1);

		mGameBoard->Swap (mSelectedCell1, mSelectedCell2);

		if (mIsCanceling)
		{
			mDisableInputCount--;
			mIsSwapping = false;
		}
		else if (mGameBoard->RemoveCombinations (mRemovedEntries))
		{
			mScore += mRemovedEntries.size();
			mScoreTextControl->SetText ("Score: " + std::to_string (mScore));

			StartExplode();
			StartDropping();

			mDisableInputCount--;
			mIsSwapping = false;
		}
		else
		{
			StartSwap (mSelectedCell1, mSelectedCell2, true);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::StartDropping (void)
{
	std::vector<int> dropInfo;

	if (mGameBoard->GetDropInfo (dropInfo)) // gets for each column the lowest empty y-coordinate, or -1 if column is full
	{
		mDisableInputCount++;
		mIsDropping = true;

		const Uint32 ticks = SDL_GetTicks();
		mDropEndTicks = ticks + sDropDuration;

		for (int x = 0; x < (int)dropInfo.size(); x++)
		{
			if (dropInfo[x] >= 0) // do we have an empty cell?
			{
				for (int y = dropInfo[x] - 1; y >= 0; y--)	// start with the lower one
				{
					mGameBoard->Swap ({ x, y }, { x, y + 1 });

					CArtifactPtr artifact = mArtifacts.Get (x, y);
					if (artifact)
						artifact->StartMoveTo (GetCellPosition ({ x, y + 1 }), ticks, mDropEndTicks);
					mArtifacts.Set (x, y + 1, artifact);
					mArtifacts.Set (x, y, nullptr);
				}

				SDL_Point topCell = { x, 0 };
				int type = mGameBoard->SetRandom (topCell);
				SDL_Point thePos = GetCellPosition ({ x, -1 });	// get the position above the top cell

				CArtifactPtr artifact = std::make_shared<CArtifact> (mStage, mArtifactTextures[type]);

				artifact->SetPosition (thePos);
				artifact->SetClip (&mClipRect);

				mArtifacts.Set (topCell, artifact);
				mStage->AddDrawable (artifact);
				artifact->StartMoveTo (GetCellPosition (topCell), ticks, mDropEndTicks);
			}
		}
	}
}


void
CGame::UpdateDropping (Uint32 inCurTicks)
{
	if (!mIsDropping)
		return;

	if (inCurTicks > mDropEndTicks)
	{
		if (mGameBoard->RemoveCombinations (mRemovedEntries))
			StartExplode();

		mDisableInputCount--;
		mIsDropping = false;

		StartDropping(); // try dropping some more!
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::Update (Uint32 inCurTicks)
{
	if (!mGameFinished)
	{
		UpdateSwap (inCurTicks);
		UpdateDropping (inCurTicks);
	}

	if (mGameRunning && inCurTicks >= mGameEndTicks)
		Finish();

	UpdateExplode (inCurTicks);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGame::HandleMouseDown (int inX, int inY)
{
	bool isInGrid = inX >= sGridLeft && inX < sGridLeft + sGridColCount * sCellWidth
				 && inY >= sGridTop && inY < sGridTop + sGridRowCount * sCellHeight;

	bool inputBlocked = mIsSwapping || mIsDropping || !mGameRunning; // mDisableInputCount > 0

	if (isInGrid && !inputBlocked)
	{
		SDL_Point p;
		p.x = (inX - sGridLeft) / sCellWidth;
		p.y = (inY - sGridTop) / sCellHeight;
		Trace ("Mouse down in cell %d, %d\n", p.x, p.y);

		if (!mHasSelection)
		{
			mHasSelection = true;
			mSelectedCell1 = p;

			CArtifactPtr artifact = mArtifacts (mSelectedCell1);
			if (artifact)
				artifact->MarkSelected (true);
		}
		else
		{
			mHasSelection = false;
			mSelectedCell2 = p;

			CArtifactPtr artifact = mArtifacts (mSelectedCell1);
			if (artifact)
				artifact->MarkSelected (false);

			// start swapping if the selected cells are neighbours
			if (abs(mSelectedCell1.x - mSelectedCell2.x) + abs(mSelectedCell1.y - mSelectedCell2.y) == 1)
				StartSwap (mSelectedCell1, mSelectedCell2, false);
			else
			{
				mHasSelection = true;
				mSelectedCell1 = p;

				CArtifactPtr artifact = mArtifacts (mSelectedCell1);
				if (artifact)
					artifact->MarkSelected (true);
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
