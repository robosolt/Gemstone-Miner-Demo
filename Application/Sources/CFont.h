// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CFONT_H__
#define __CFONT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SDL_ttf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Container class for a TTF_Font
//
//	An TTF_Font is not a real object, so it has no constructor or destructor
//	This class makes live a bit easier, so you can use smart pointers and stuff

class CFont
{
public:
								CFont (const std::string inFilePath, int inPtSize, int inIndex = 0);
								CFont (TTF_Font *inFont);				// takes ownership of the TTF_Font
								~CFont (void);

								operator TTF_Font * ();					// cast to TTF_Font

								CFont (const CFont&) = delete;			// don't allow copying through copy constructor
	CFont &						operator = (const CFont&) = delete;		// don't allow copying through assignment

private:

	TTF_Font *					mTTFFont;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CFONT_H__
