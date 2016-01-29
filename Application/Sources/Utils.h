// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __UTILS_H__
#define __UTILS_H__

////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T> inline const T sqr (const T &a) { return a * a; }
template <typename T> inline const T cube (const T &a) { return a * a * a; }
template <typename T> inline const T clip (const T x, const T a, const T b) { return (a < b) ? ((x <= a) ? a : (x >= b) ? b : x) : ((x <= b) ? b : (x >= a) ? a : x); }
template <typename T> inline const T wrap (const T x, const T a, const T b) { return x >= 0 ? fmod(x,b-a)+a : fmod(x,b-a)+b; }


////////////////////////////////////////////////////////////////////////////////////////////////////

	
namespace Utils
{
	// no utils here at this moment
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#if _DEBUG
//	Facilities to do printf() style output to debug window
void Trace (const char *inFormat, ...);
void TraceIf (bool inCondition, const char *inFormat, ...);
#else
#define Trace(...)
#define TraceIf(...)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __UTILS_H__
