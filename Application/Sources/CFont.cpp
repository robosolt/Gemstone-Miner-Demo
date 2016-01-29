// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CFont.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CFont::CFont (const std::string inFilePath, int inPtSize, int inIndex) :
	mTTFFont (TTF_OpenFont (inFilePath.c_str(), inPtSize))
{
}


CFont::CFont (TTF_Font *inFont) :
	mTTFFont (inFont)
{
}


CFont::~CFont (void)
{
	TTF_CloseFont (mTTFFont);
	mTTFFont = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*

CFont &
CFont::operator = (TTF_Font *inFont)
{
	TTF_CloseFont (mTTFFont);
	mTTFFont = inFont;

	return *this;
}

*/
////////////////////////////////////////////////////////////////////////////////////////////////////


CFont::operator TTF_Font * ()
{
	return mTTFFont;
}

