// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

////////////////////////////////////////////////////////////////////////////////////////////////////

// See PrecHeader.cpp for comments

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <random>
#include "SDL.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

#define GAME_TITLE "Gemstone Miner"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	common classes and their pointer types
//
//	shared_ptrs are put here in stead of in the corresponding .h file to avoid having to include
//	the .h file when all we need is the pointer type, without the whole class declaration.
//	This reduces build dependencies.
//
//	if this were a large application, we should put this in a separate include file (CommonTypes.h)


class CGame;
typedef std::shared_ptr<CGame> CGamePtr;

class CStage;
typedef std::shared_ptr<CStage> CStagePtr;

class CRenderer;
typedef std::shared_ptr<CRenderer> CRendererPtr;

class CTexture;
typedef std::shared_ptr<CTexture> CTexturePtr;

class CDrawable;
typedef std::shared_ptr<CDrawable> CDrawablePtr;

class CSprite;
typedef std::shared_ptr<CSprite> CSpritePtr;

class CAnimatedSprite;
typedef std::shared_ptr<CAnimatedSprite> CAnimatedSpritePtr;

class CFont;
typedef std::shared_ptr<CFont> CFontPtr;

class CTextControl;
typedef std::shared_ptr<CTextControl> CTextControlPtr;

class CArtifact;
typedef std::shared_ptr<CArtifact> CArtifactPtr;

class CGameBoard;
typedef std::shared_ptr<CGameBoard> CGameBoardPtr;

template <typename T> class CInterpolator2D;
typedef std::shared_ptr<CInterpolator2D<SDL_Point>> CInterpolator2DPtr;


////////////////////////////////////////////////////////////////////////////////////////////////////
