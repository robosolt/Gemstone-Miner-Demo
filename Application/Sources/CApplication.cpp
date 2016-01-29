// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CApplication.h"
#include "CStage.h"
#include "CGame.h"
#include "CFont.h"
#include "Events.h"

#include "SDL_timer.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CApplication::CApplication (const std::string inAppTitle) :
	mAppTitle (inAppTitle),
	mStage (nullptr),
	mWindowWidth (755),
	mWindowHeight (600),
	mQuitRequested (false)
{
	Initialize();
}


CApplication::~CApplication (void)
{
	Finalize();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

							
void
CApplication::Initialize (void)
{
	// initialize the SDL library
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		throw (std::string("SDL_Init Error: ") + SDL_GetError());
	
	// load support for necessary image formats
	const int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init (img_flags) != img_flags)
		throw (IMG_GetError());

	// load support for fonts
	if (TTF_Init() == -1) 
		throw (TTF_GetError());

	// load support for sound mixer
	if (Mix_OpenAudio (22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 )
		throw (Mix_GetError());

	// init path to app executable
	const char *basePath = SDL_GetBasePath();
	mAppPath = basePath;
	SDL_free((void*)basePath);

	mDefaultFont = std::make_shared<CFont> (mAppPath + "Assets\\Fonts\\freefont\\FreeSans.ttf", 12);

	mStage = std::make_shared<CStage> (mAppTitle.c_str(), mWindowWidth, mWindowHeight, mDefaultFont);
#if _DEBUG
	mStage->ShowHud();
#endif

	mGame = std::make_shared<CGame> (mStage, mAppPath + "Assets\\");
}


void
CApplication::Finalize (void)
{
	mStage->Finalize();

	// the following objects may have dependencies on the SDL libraries,
	// so we should explicitely remove them before quiting  the libs
	mStage = nullptr;
	mGame = nullptr;
	mDefaultFont = nullptr;

	Mix_CloseAudio();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CApplication::Run (void)
{
	SDL_Event sdlEvent;

	const unsigned int fps = 50;
	bool firstGame = true;

	mStage->StartRenderTimer (fps);
	mGame->Prepare();

	while (!mQuitRequested)
	{
		if (SDL_WaitEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					RequestQuit();
					break;
				case SDL_MOUSEBUTTONDOWN:
					mGame->HandleMouseDown (sdlEvent.button.x, sdlEvent.button.y);
					break;
				case SDL_KEYDOWN:
					switch (sdlEvent.key.keysym.sym)
					{
						case SDLK_h: // toggle hud
							if (!sdlEvent.key.repeat)
								mStage->ToggleHud();
							break;

						case SDLK_n: // start game
							if (!sdlEvent.key.repeat && !mGame->IsRunning())
							{
								// after the first game we have to prepare a new game before starting
								if (!firstGame)
									mGame->Prepare();

								mGame->Start();

								firstGame = false;
							}
							break;
#if _WIN32
						case SDLK_F4: // quit under windows
							if (sdlEvent.key.keysym.mod == KMOD_LALT && !sdlEvent.key.repeat)
								RequestQuit();
							break;
#endif
					}
					break;
				case USEREVENT_RENDER_REQUEST:
					// first we render, then we use all the time we need to update the game and the stage
					// if we first update, then this may take variable time and we don't render on time
					mStage->Render();

					Uint32 nextTicks = SDL_GetTicks() + 1000/fps;
					mGame->Update (nextTicks);		// update the game logic
					mStage->Update (nextTicks);		// update animations
					break;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CApplication::RequestQuit (void)
{
	mQuitRequested = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
