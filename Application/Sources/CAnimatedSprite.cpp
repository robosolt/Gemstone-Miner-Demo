// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CAnimatedSprite.h"
#include "CStage.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CStage.h"
#include "CInterpolator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CAnimatedSprite::CAnimatedSprite (CStagePtr inStage, const std::vector<std::string> &inFileNames, const SDL_Rect *inClipRect) :
	CSprite (inStage, nullptr, inClipRect),
	mLastUpdateTicks (0),
	mCurrentTextureIndex (0)
{
	for (unsigned int i = 0; i < inFileNames.size(); i++)
	{
		CTexturePtr texture = mStage->CreateTexture (inFileNames[i]);
		if (texture)
			mTextures.push_back (texture);
	}

	SelectTexture (0);
}


CAnimatedSprite::~CAnimatedSprite (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CAnimatedSprite::SelectTexture (unsigned int inIndex)
{
	if (mTextures.size() > 0)
	{
		// select next texture
		mCurrentTextureIndex = inIndex % mTextures.size ();
		mTexture = mTextures[mCurrentTextureIndex];

		// get the size of the new texture
		SDL_QueryTexture (*mTexture, nullptr, nullptr, &mWidth, &mHeight);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CAnimatedSprite::Update (Uint32 inCurTicks)
{
	// update as a norma sprite
	CSprite::Update (inCurTicks);

	// but change the texture when the time has come
	if (inCurTicks > mLastUpdateTicks + 1000 / mFps)
	{
		mLastUpdateTicks = inCurTicks;

		SelectTexture (mCurrentTextureIndex + 1);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////

