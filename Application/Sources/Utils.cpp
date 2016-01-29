// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG

//	Facilities to do printf() style debug output

void
Trace (const char *inFormat, ...)
{
	va_list args;
	va_start (args, inFormat);
	char buffer[512];
	memset (buffer, 0, sizeof(buffer));
#if _WIN32
	vsnprintf_s (buffer, sizeof(buffer)-1, inFormat, args);
#else
	vsnprintf (buffer, sizeof(buffer)-1, inFormat, args);
#endif
	buffer[sizeof(buffer) - 1] = '\0';

#if _WIN32
	_RPT0 (_CRT_WARN, "*** ");
	_RPT0 (_CRT_WARN, buffer);
#else
	#error NOT IMPLEMENTED: Output to debug for platforms other than Win32
#endif
}


void TraceIf (bool inCondition, const char *inFormat, ...)
{
	if (inCondition)
	{
		va_list args;
		va_start (args, inFormat);
		char buffer[512];
		memset (buffer, 0, sizeof(buffer));
#if _WIN32
		vsnprintf_s (buffer, sizeof(buffer)-1, inFormat, args);
#else
		vsnprintf (buffer, sizeof(buffer)-1, inFormat, args);
#endif
		buffer[sizeof(buffer) - 1] = '\0';

#if _WIN32
		_RPT0 (_CRT_WARN, "*** ");
		_RPT0 (_CRT_WARN, buffer);
#else
	#error NOT IMPLEMENTED: Output to debug for platforms other than Win32
#endif
	}
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////


