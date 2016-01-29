// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CStage.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CArtifact.h"
#include "CGameBoard.h"
#include "CFont.h"
#include "CTextControl.h"
#include "Events.h"
#include "SDL_image.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CStage::CStage (const std::string inWindowTitle, unsigned int inWidth, unsigned int inHeight, CFontPtr inDefaultFont) :
	mMainWindow (nullptr),
	mRenderer (nullptr),
	mDefaultFont (inDefaultFont),
	mRenderTimerID (0),
	mLaggingBehind (false),
	mFrameRate (0),
	mShowHud (false)
{
	// create the main window
	mMainWindow = SDL_CreateWindow (inWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, inWidth, inHeight, SDL_WINDOW_SHOWN);
	if (!mMainWindow)
		throw (std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	// create the main renderer
	#if 0
		// software renderer for slow machine testing (althought this may break some rendering features on some machines)
		mRenderer = std::make_shared<CRenderer> (SDL_CreateRenderer (mMainWindow, -1, SDL_RENDERER_SOFTWARE));
	#else
		mRenderer = std::make_shared<CRenderer> (SDL_CreateRenderer (mMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	#endif
	if (!mRenderer)
		throw (std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}


CStage::~CStage (void)
{
	Finalize();
	SDL_DestroyWindow (mMainWindow);
	mMainWindow = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::Finalize (void)
{
	StopRenderTimer();

	// remove all drawables that have circular references to this object
	mLayers.clear();
	mBackground = 0;
	mHud = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


CTexturePtr
CStage::CreateTexture (const std::string inFileName)
{
	return std::make_shared<CTexture> (mRenderer, inFileName);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::AddDrawable (CDrawablePtr inDrawable, unsigned int inZIndex)
{
	if (inZIndex >= mLayers.size())
		mLayers.resize (inZIndex+1);

	mLayers[inZIndex].push_back (inDrawable);
}


void
CStage::RemoveDrawable (CDrawablePtr inDrawable)
{
	for (unsigned int i = 0; i < mLayers.size(); i++)
		mLayers[i].remove (inDrawable);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::SetBackground (const std::string inFileName)
{
	mBackground = CreateTexture (inFileName);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


bool
CStage::ShowHud (bool inShow)
{
	bool prevSetting = mShowHud;

	mShowHud = inShow;

	if (mShowHud && !mHud)
	{
		// note: we cannot create the hud in the contructor, since we need to pass the stage (the
		// this pointer) as a parameter to the CTextControl, which is not yet set at that time

		SDL_Color hudTextColor = { 255, 255, 255 };
		mHud = std::make_shared<CTextControl> (shared_from_this(), 10, 10, mDefaultFont, hudTextColor);
	}

	return prevSetting;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::StartRenderTimer (unsigned int inFrameRate)
{
	StopRenderTimer();

	mFrameRate = inFrameRate;

	mRenderTimerID = SDL_AddTimer (1000 / mFrameRate, CStage::RenderTimerCallback, &this->mLaggingBehind);
}


void
CStage::StopRenderTimer (void)
{
	SDL_RemoveTimer (mRenderTimerID);
	mRenderTimerID = 0;
	mLaggingBehind = false;
}


Uint32 // static
CStage::RenderTimerCallback (Uint32 inInterval, void *inParam)
{
	bool &mLaggingBehind = *(bool*) inParam;

	// if the previous event is still in the queue we're lagging behind
	if (SDL_HasEvent (USEREVENT_RENDER_REQUEST))
	{
		mLaggingBehind = true;

		// check again as soon as possible to see if it has been handled
		// so we can set-up a new timer event
		return 1;
	}

	// we should not push a new event if we just caught up with rendering
	if (!mLaggingBehind)
	{
		SDL_Event event;
		event.type = USEREVENT_RENDER_REQUEST;
		event.user.code = 0;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;

		SDL_FlushEvent (USEREVENT_RENDER_REQUEST);
		SDL_PushEvent (&event);
	}

	mLaggingBehind = false;

	return inInterval; // causes the timer to restart with same interval as before
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::Update (unsigned int inCurTicks)
{
	for (unsigned int i = 0; i < mLayers.size (); i++)
	{
		std::list<CDrawablePtr> &drawables = mLayers[i]; // somehow the VC compiler does not like having mLayers[i] directly in the 'for'
		for (std::list<CDrawablePtr>::iterator i = drawables.begin(); i != drawables.end (); i++)
			(*i)->Update (inCurTicks);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CStage::Render (void)
{
	SDL_RenderSetClipRect (*mRenderer, nullptr);
	if (mBackground)
		SDL_RenderCopy (*mRenderer, *mBackground, nullptr, nullptr);
	else
		SDL_RenderClear (*mRenderer);

	for (unsigned int i = 0; i < mLayers.size (); i++)
	{
		std::list<CDrawablePtr> &drawables = mLayers[i]; // somehow the VC compiler does not like having mLayers[i] directly in the 'for'
		for (std::list<CDrawablePtr>::iterator i = drawables.begin (); i != drawables.end (); i++)
			(*i)->Render ();
	}

	if (mShowHud && mHud)
	{
		static unsigned int ticks = 0;
		unsigned int new_ticks = SDL_GetTicks ();
		unsigned int frametime = new_ticks - ticks;
		unsigned int fps = 1000 / frametime;
		ticks = new_ticks;

		mHud->SetText ("FPS: " + std::to_string (fps));
		mHud->Render();
	}
	
	SDL_RenderPresent (*mRenderer);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
