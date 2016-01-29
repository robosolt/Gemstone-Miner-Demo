// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CINTERPOLATOR_H__
#define __CINTERPOLATOR_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Interpolators
//
//	Interpolate between values or positions
//	if the interval start/end values are not specified, then the fraction passed to GetPosition()
//	maps 0.0 to the start value, and 1.0 to the end value.
//	if the interval start/end values are specified, then the fraction will first be remapped from
//	the given interval to the interval [0.0 and 1.0]. This allows you to have a separate reference
//	frame (like time).
//
//	Currently the CInterpolator and CInterpolator2D classes do linear interpolation.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class CInterpolator
{
public:
								CInterpolator (T inStartValue, T inEndValue, double inIntervalStart = 0.0, double inIntervalEnd = 1.0);
	virtual						~CInterpolator (void) { }

	virtual T					GetPosition (double inFraction, bool inClip) const = 0;
	virtual T					GetEndPosition (void) = 0;

protected:

	T							mStartValue;
	T							mEndValue;
	double						mIntervalStart;
	double						mIntervalEnd;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Type T should have a public numeric x and y member (int, double, ...)

template <typename T>
class CInterpolator2D
{
public:
								CInterpolator2D (T inStart, T inEnd, double inIntervalStart = 0.0, double inIntervalEnd = 1.0);
								CInterpolator2D (const std::vector<T> &inPath, double inIntervalStart = 0.0, double inIntervalEnd = 1.0);
	virtual						~CInterpolator2D (void) { }

	virtual T					GetPosition (double inFraction, bool inClip) const;
	virtual T					GetEndPosition (void) const;

protected:

	const std::vector<T>		mPath;

	double						mIntervalStart;
	double						mIntervalEnd;
	double						mTotalLength;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CInterpolator.cpp"

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CINTERPOLATOR_H__
