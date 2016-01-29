// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CDrawable.h"
#include "CStage.h"
#include "CRenderer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CDrawable::CDrawable (CStagePtr inStage, int inX, int inY, const SDL_Rect *inClipRect) :
	mStage (inStage),
	mX (inX),
	mY (inY),
	mUseClipRect (inClipRect != nullptr)
{
	if (inClipRect)
		mClipRect = *inClipRect;
}


CDrawable::~CDrawable (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CDrawable::PrepareClip (void)
{
	SDL_RenderSetClipRect (*mStage->GetRenderer(), mUseClipRect ? &mClipRect : nullptr);
}


void
CDrawable::Render (void)
{
	// override this to get something drawn
}


////////////////////////////////////////////////////////////////////////////////////////////////////
