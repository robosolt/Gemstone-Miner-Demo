// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CSprite.h"
#include "CStage.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CStage.h"
#include "CInterpolator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CSprite::CSprite (CStagePtr inStage, CTexturePtr inTexture, const SDL_Rect *inClipRect) :
	CDrawable (inStage, 0, 0, inClipRect),
	mTexture (inTexture),
	mInterpolator (nullptr),
	mWidth (0),
	mHeight (0)
{
	if (inTexture)
		SDL_QueryTexture (*inTexture, nullptr, nullptr, &mWidth, &mHeight);
}


CSprite::~CSprite (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CSprite::SetInterpolator (CInterpolator2DPtr inInterpolator)
{
	mInterpolator = inInterpolator;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CSprite::StartMoveTo (SDL_Point inEndPt, Uint32 inStartTicks, Uint32 inEndTicks)
{
	mInterpolator = std::make_shared<CInterpolator2D<SDL_Point>> (SDL_Point ({ mX, mY }), inEndPt, inStartTicks, inEndTicks);
}


void
CSprite::SetPath (const std::vector<SDL_Point> &inPath, Uint32 inStartTicks, Uint32 inEndTicks)
{
	mInterpolator = std::make_shared<CInterpolator2D<SDL_Point>> (inPath, inStartTicks, inEndTicks);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CSprite::FinishMoveNow (void)
{
	if (mInterpolator)
	{
		SDL_Point p = mInterpolator->GetEndPosition();
		mX = p.x;
		mY = p.y;
		mInterpolator = 0;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CSprite::Update (Uint32 inCurTicks)
{
	if (mInterpolator)
	{
		SDL_Point p = mInterpolator->GetPosition (inCurTicks, true);
		mX = p.x;
		mY = p.y;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CSprite::Render (void)
{
	if (mWidth > 0 && mHeight > 0)
	{
		SDL_Rect dstRect = { mX, mY, mWidth, mHeight };

		PrepareClip ();

		SDL_RenderCopy (*mStage->GetRenderer (), *mTexture, nullptr, &dstRect);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////

