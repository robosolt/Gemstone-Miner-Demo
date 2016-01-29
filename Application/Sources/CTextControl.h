// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CTEXTCONTROL_H__
#define __CTEXTCONTROL_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CDrawable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	A simple text control; single font-family/size/color

class CTextControl :
	public CDrawable
{
public:
								CTextControl (CStagePtr inStage, int inX, int inY, const CFontPtr inFont, const SDL_Color &inColor = { 0, 0, 0 });
	virtual						~CTextControl (void);

	virtual void				Render (void);

	virtual void				SetText (const std::string inText);

protected:

	std::string					mText;
	unsigned long				mWidth;
	unsigned long				mHeight;
	CFontPtr					mFont;
	SDL_Color					mColor;
	CTexturePtr					mTexture;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CTEXTCONTROL_H__
