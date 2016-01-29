// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CDRAWABLE_H__
#define __CDRAWABLE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CDrawable
{
public:
								CDrawable (CStagePtr inStage, int inX = 0, int inY = 0, const SDL_Rect *inClipRect = nullptr);
	virtual						~CDrawable (void);

	virtual void				Update (Uint32 inCurTicks) { }
	virtual void				Render (void);

	virtual void				SetPosition (const SDL_Point &inPt) { mX = inPt.x; mY = inPt.y; }

	virtual void				SetClip (SDL_Rect *inRect)	{ mUseClipRect = inRect != nullptr; if (mUseClipRect) mClipRect = *inRect; }
	virtual SDL_Rect			GetClip (void) const		{ return mClipRect; }
	virtual bool				HasClip (void) const		{ return mUseClipRect; }

protected:

	void						PrepareClip (void);

	CStagePtr					mStage;
	SDL_Rect					mClipRect;
	bool						mUseClipRect;

	int							mX;
	int							mY;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CDRAWABLE_H__
