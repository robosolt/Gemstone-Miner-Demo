// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CARTIFACT_H__
#define __CARTIFACT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CSprite.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class CArtifact :
	public CSprite
{
public:
								CArtifact (CStagePtr inStage, CTexturePtr inTexture);
	virtual						~CArtifact (void);

	virtual void				MarkSelected (bool inSelected) { /* TO DO: change the appearance of this artifact to mark it's selected */ }
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CARTIFACT_H__
