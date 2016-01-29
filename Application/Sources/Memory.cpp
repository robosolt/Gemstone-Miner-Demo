// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "Memory.h"

#if _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////


void
Memory::Initialize (void)
{
	// setup memory leak detection
#if _WIN32 && _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}


void
Memory::Finalize (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
