// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CSPRITE_H__
#define __CSPRITE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CDrawable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class CSprite :
	public CDrawable
{
public:
								CSprite (CStagePtr inStage, CTexturePtr inTexture, const SDL_Rect *inClipRect = 0);
	virtual						~CSprite (void);

								// set interpolator; will be used to update position when calling Update()
	virtual void				SetInterpolator (CInterpolator2DPtr inAnimator);

								// set interpolator to move to the specified end position
	virtual void				StartMoveTo (SDL_Point inEndPt, Uint32 inStartTicks, Uint32 inEndTicks);
	virtual void				FinishMoveNow (void);

	virtual void				SetPath (const std::vector<SDL_Point> &inPath, Uint32 inStartTicks, Uint32 inEndTicks);

	virtual void				Stop (void) { mInterpolator = 0; }

	virtual void				Update (Uint32 inCurTicks);
	virtual void				Render (void);

protected:

	CTexturePtr					mTexture;

	int							mWidth;
	int							mHeight;

	CInterpolator2DPtr			mInterpolator;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CSPRITE_H__
