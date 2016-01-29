// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CTEXTURE_H__
#define __CTEXTURE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Container class for a SDL_Texture
//
//	An SDL_Texture is not a real object, so it has no constructor or destructor
//	This class makes live a bit easier, so you can use smart pointers and stuff

class CTexture
{
public:
								CTexture (CRendererPtr inRenderer, const std::string inFileName);
								CTexture (SDL_Texture *inTexture);			// takes ownership of the SDL_Texture
								~CTexture (void);

								operator SDL_Texture * ();					// cast to SDL_Texture

								CTexture (const CTexture&) = delete;		// don't allow copying through copy constructor
	CTexture &					operator = (const CTexture&) = delete;		// don't allow copying through assignment

private:

	SDL_Texture *				mSDLTexture;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CTEXTURE_H__
