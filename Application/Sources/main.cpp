// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CApplication.h"
#include "Memory.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


int main (int argc, char **argv)
{
	Memory::Initialize();

	try
	{
		CApplication app (GAME_TITLE);
		app.Run();
	}
	catch (const char *errorMessage)
	{
		// alert the user of the error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GAME_TITLE, errorMessage, NULL);
	}
	catch (std::string errorMessage)
	{
		// alert the user of the error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GAME_TITLE, errorMessage.c_str(), NULL);
	}
	catch (...)
	{
		// alert the user of the error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GAME_TITLE, "Oops! The application has encountered an unrecoverable error and will shut down.", NULL);
	}

	Memory::Finalize();

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////