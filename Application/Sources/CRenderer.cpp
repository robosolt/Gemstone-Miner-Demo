// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CRenderer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


CRenderer::CRenderer (SDL_Renderer *inRenderer) :
	mSDLRenderer (inRenderer)
{
}


CRenderer::~CRenderer (void)
{
	SDL_DestroyRenderer (mSDLRenderer);
	mSDLRenderer = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*

CRenderer &
CRenderer::operator = (SDL_Renderer *inRenderer)
{
	SDL_DestroyRenderer (mSDLRenderer);
	mSDLRenderer = inRenderer;

	return *this;
}

*/
////////////////////////////////////////////////////////////////////////////////////////////////////


CRenderer::operator SDL_Renderer * ()
{
	return mSDLRenderer;
}

