// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CGAME_H__
#define __CGAME_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CMatrix.h"

struct Mix_Chunk;

////////////////////////////////////////////////////////////////////////////////////////////////////


class CGame
{
public:
								CGame (CStagePtr inStage, const std::string inAssetsPath);
								~CGame (void);

	void						Prepare (void);			// prepare the gameboard for playing (may do animations)
	void						Start (void);			// start timer and enable user input
	void						Finish (void);			// force-finish the game (for show only)

	bool						IsRunning (void) { return mGameRunning; }

	void						StartSwap (SDL_Point inPt1, SDL_Point inPt2, bool inCanceling);

	void						ExplodeCombinations (void);

	void						Drop (unsigned int inColumnIndex, CArtifact *inArtifact);
	void						Remove (unsigned int inColumnIndex, unsigned int inRowIndex);

	void						Update (unsigned int inFps);

	void						HandleMouseDown (int inX, int inY);

	void						ShowAlert (std::string inText, SDL_Point *inPos = 0);
	void						HideAlert (void);

private:

	void						CreateTextures (void);
	void						CreateObjects (void);

	void						LoadSoundFx (void);

	void						UpdateSwap (Uint32 inCurTicks);

	void						StartExplode (void);
	void						UpdateExplode (Uint32 inCurTicks);

	void						StartDropping (void);
	void						UpdateDropping (Uint32 inCurTicks);

	void						StartFuse (Uint32 inCurTicks);
	void						StopFuse (void);

	void						RemoveArtifact (const SDL_Point &inPt);

	SDL_Point					GetCellPosition (const SDL_Point &inPt);

	std::string					mAssetsPath;

	CStagePtr					mStage;
	CGameBoardPtr				mGameBoard;
	std::vector<CTexturePtr>	mArtifactTextures;
	CMatrix<CArtifactPtr>		mArtifacts;

	SDL_Rect					mClipRect;

	int							mTypeCount;

	std::vector<CSpritePtr>		mSprites;

	int							mDisableInputCount;		// there are several operations that may disable the input (currently not used)

	bool						mIsExploding;			// explosions do not block the user input
	std::list<SDL_Point>		mRemovedEntries;		// these are the entries that are being removed

	bool						mIsDropping;
	Uint32						mDropEndTicks;			// when the dropping will be finished

	bool						mHasSelection;			// did user select a cell yet?
	bool						mIsSwapping;			// animation in progress
	bool						mIsCanceling;			// only relevant when mIsSwapping
	SDL_Point					mSelectedCell1;			// first cell clicked by user
	SDL_Point					mSelectedCell2;			// second cell clicked by user
	CArtifactPtr				mArtifact1;				// corresponding to mSelectedCell1
	CArtifactPtr				mArtifact2;				// corresponding to mSelectedCell1
	Uint32						mSwapEndTicks;			// when the swapping will be finished

	bool						mGameRunning;			// user input is disable when not started
	bool						mGameFinished;			// game is over
	Uint32						mGameEndTicks;			// when the game time is over

	std::vector<Mix_Chunk*>		mSoundFxBreak;
	Mix_Chunk *					mSoundFxFuse;
	Mix_Chunk *					mSoundFxExplosion;
	int							mFuseChannel;

	CAnimatedSpritePtr			mFuse;

	CTextControlPtr				mAlertTextControl;
	CFontPtr					mAlertFont;

	CTextControlPtr				mScoreTextControl;
	CFontPtr					mScoreFont;
	int							mScore;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CGAME_H__
