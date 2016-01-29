// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CTextControl.h"
#include "CStage.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CFont.h"
#include "SDL_ttf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CTextControl::CTextControl (CStagePtr inStage, int inX, int inY, const CFontPtr inFont, const SDL_Color &inColor) :
	CDrawable (inStage, inX, inY),
	mFont (inFont),
	mColor (inColor)
{
}


CTextControl::~CTextControl (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CTextControl::Render (void)
{
	PrepareClip ();

	SDL_Rect copyRect = { mX, mY, mWidth, mHeight };

	SDL_RenderCopy (*mStage->GetRenderer(), *mTexture, nullptr, &copyRect);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CTextControl::SetText (const std::string inText)
{
	if (inText != mText)
	{
		mText = inText;

		SDL_Surface *tmpSurface = TTF_RenderUTF8_Blended (*mFont, mText.c_str(), mColor);

		mTexture = std::make_shared<CTexture> (SDL_CreateTextureFromSurface (*mStage->GetRenderer(), tmpSurface));

		mWidth = tmpSurface->w;
		mHeight = tmpSurface->h;

		SDL_FreeSurface(tmpSurface);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
