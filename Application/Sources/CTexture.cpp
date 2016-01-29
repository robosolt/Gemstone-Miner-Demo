// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "SDL_image.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CTexture::CTexture (CRendererPtr inRenderer, const std::string inFileName)
{
	SDL_Surface *image = IMG_Load (inFileName.c_str());
	if (!image)
		throw (std::string("IMG_Load Error: ") + SDL_GetError());

	mSDLTexture = SDL_CreateTextureFromSurface (*inRenderer, image);

	SDL_FreeSurface (image);
	if (!mSDLTexture)
		throw (std::string("SDL_CreateTextureFromSurface Error: ") + SDL_GetError());
}


CTexture::CTexture (SDL_Texture *inTexture) :
	mSDLTexture (inTexture)
{
}


CTexture::~CTexture (void)
{
	SDL_DestroyTexture (mSDLTexture);
	mSDLTexture = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*

CTexture &
CTexture::operator = (SDL_Texture *inTexture)
{
	SDL_DestroyTexture (mSDLTexture);
	mSDLTexture = inTexture;

	return *this;
}

*/
////////////////////////////////////////////////////////////////////////////////////////////////////


CTexture::operator SDL_Texture * ()
{
	return mSDLTexture;
}


