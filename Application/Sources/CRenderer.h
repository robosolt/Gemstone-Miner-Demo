// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CRENDERER_H__
#define __CRENDERER_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

struct SDL_Renderer;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Container class for a SDL_Renderer
//
//	An SDL_Renderer is not a real object, so it has no constructor or destructor
//	This class makes live a bit easier, so you can use smart pointers and stuff

class CRenderer
{
public:
								CRenderer (SDL_Renderer *inRenderer);		// takes ownership of the SDL_Renderer
								~CRenderer (void);

								operator SDL_Renderer * ();					// cast to SDL_Renderer

								CRenderer (const CRenderer&) = delete;		// don't allow copying through copy constructor
	CRenderer &					operator = (const CRenderer&) = delete;		// don't allow copying through assignment

private:

	SDL_Renderer *				mSDLRenderer;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CRENDERER_H__
