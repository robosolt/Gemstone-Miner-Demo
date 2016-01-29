// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CANIMATEDSPRITE_H__
#define __CANIMATEDSPRITE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CSprite.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class CAnimatedSprite :
	public CSprite
{
public:
								CAnimatedSprite (CStagePtr inStage, const std::vector<std::string> &inFileNames, const SDL_Rect *inClipRect = 0);
	virtual						~CAnimatedSprite (void);

	virtual void				Update (Uint32 inCurTicks);

	void						SetFps (unsigned int inFps) { mFps = inFps; }

protected:

	void						SelectTexture (unsigned int inIndex);

	std::vector<CTexturePtr>	mTextures;
	unsigned int				mCurrentTextureIndex;

	unsigned int				mFps;
	Uint32						mLastUpdateTicks;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CANIMATEDSPRITE_H__
