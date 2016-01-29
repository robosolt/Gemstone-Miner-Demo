// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 13-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
CInterpolator<T>::CInterpolator (T inStartValue, T inEndValue, double inIntervalStart, double inIntervalEnd) :
	mStartValue (inStartValue),
	mEndValue (inEndValue),
	mIntervalStart (inIntervalStart),
	mIntervalEnd (inIntervalEnd)
{
}


template <typename T>
T
CInterpolator<T>::GetPosition (double inFraction, bool inClip) const
{
	double realFraction = inFraction / (mIntervalEnd - mIntervalStart);

	T value = static_cast<T> (mStartValue + realFraction * (mEndValue - mStartValue));

	return inClip ? clip (value, mStartValue, mEndValue) : value;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
CInterpolator2D<T>::CInterpolator2D (T inStart, T inEnd, double inIntervalStart, double inIntervalEnd) :
	CInterpolator2D<T> (std::vector<T> ({ inStart, inEnd }), inIntervalStart, inIntervalEnd)
{
}


template <typename T>
CInterpolator2D<T>::CInterpolator2D (const std::vector<T> &inPath, double inIntervalStart, double inIntervalEnd) :
	mPath (inPath),
	mIntervalStart (inIntervalStart),
	mIntervalEnd (inIntervalEnd),
	mTotalLength (0)
{
	mTotalLength = 0.0;
	for (unsigned int i = 0; i < mPath.size()-1; i++)
		mTotalLength += sqrt (sqr(inPath[i].x - inPath[i+1].x) + sqr(inPath[i].y - inPath[i+1].y));
}


template <typename T>
T
CInterpolator2D<T>::GetPosition (double inFraction, bool inClip) const
{
	unsigned int size = mPath.size();

	// quick exit
	if (size == 0)
		return { 0, 0 };

	double realFraction = (inFraction - mIntervalStart) / (mIntervalEnd - mIntervalStart);

	// quick exit
	if (size == 1 || realFraction == 0 || (realFraction < 0 && inClip))
		return mPath[0];

	// quick exit
	if (realFraction == 1 || (realFraction > 1 && inClip))
		return mPath.back();

	unsigned int startIndex = 0;

	// for now, assume that all segments are the same length; this may cause different
	// speeds at different segments if this is not the case
	double segmentLength = mTotalLength / (size-1);

	if (realFraction < 0)
	{
		// extrapolate first line segment
		startIndex = 0;
	}
	else if (realFraction > 1)
	{
		// extrapolate last line segment
		startIndex = size - 2;
	}
	else
	{
		// get the corresponding segment
		startIndex = static_cast<int>(realFraction * (size-1));

		// account for rounding errors
		if (startIndex > size - 2)
			startIndex = size - 2;
	}

	T start = mPath[startIndex];
	T end = mPath[startIndex + 1];

	// scale-up realFraction to reflect only this segment
	realFraction = (size-1) * realFraction - startIndex;

	T pos;
	pos.x = static_cast<decltype(pos.x)> (start.x + realFraction * (end.x - start.x));
	pos.y = static_cast<decltype(pos.x)> (start.y + realFraction * (end.y - start.y));

	return pos;
}


template <typename T>
T
CInterpolator2D<T>::GetEndPosition (void) const
{
	return mPath.empty() ? T ({0, 0}) : mPath.back();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
