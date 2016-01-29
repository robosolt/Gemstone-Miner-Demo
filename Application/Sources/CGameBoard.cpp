// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#include "PrecHeaders.h"
#include "CGameBoard.h"
#include "CArtifact.h"
#include "CInterpolator.h"
#include "CTexture.h"
#include "CStage.h"
#include <random>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////////////////////////

const int CGameBoard::sEmpty = -1;

////////////////////////////////////////////////////////////////////////////////////////////////////


CGameBoard::CGameBoard (unsigned int inColCount, unsigned int inRowCount, unsigned int inTypeCount) :
	CMatrix (inColCount, inRowCount),
	mTypeCount (inTypeCount),
	mMinRunLength (3),
	mRandomGenerator ((unsigned int) std::chrono::system_clock::now().time_since_epoch().count())
{
	// initialize the random generator to return a number within the range of the number of types
	mTypesDistribution.param (std::uniform_int_distribution<int>::param_type (0, inTypeCount - 1 ));

	Clear();
}


CGameBoard::~CGameBoard (void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int
CGameBoard::GetRandomType (void)
{
	return mTypesDistribution (mRandomGenerator);
}


int
CGameBoard::SetRandom (const SDL_Point &inPt)
{
	int type = GetRandomType();

	Set (inPt, type);

	return type;
}


bool
CGameBoard::IsEmpty (const SDL_Point &inPt) const
{
	return Get(inPt) == sEmpty;
}


void
CGameBoard::Swap (const SDL_Point &inPt1, const SDL_Point &inPt2)
{
	std::swap (operator()(inPt1), operator()(inPt2));
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void
CGameBoard::Fill (void)
{
	for (unsigned int y = 0; y < mHeight; y++)
		for (unsigned int x = 0; x < mWidth; x++)
			Set (x, y, GetRandomType());
}


void
CGameBoard::Clear (void)
{
	for (unsigned int y = 0; y < mHeight; y++)
		for (unsigned int x = 0; x < mWidth; x++)
			Set (x, y, sEmpty);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/* No longer used

bool
CGameBoard::CanSwap (const SDL_Point &inPt1, const SDL_Point &inPt2)
{
	return HasCombination (inPt1) || HasCombination (inPt2);
}


bool
CGameBoard::HasCombination (const SDL_Point &inPt)
{
	return GetRunLength (inPt, -1, 0) + GetRunLength (inPt, 1, 0) - 1 >= mMinRunLength
		|| GetRunLength (inPt, 0, -1) + GetRunLength (inPt, 0, 1) - 1 >= mMinRunLength;
}

*/
////////////////////////////////////////////////////////////////////////////////////////////////////


int
CGameBoard::GetRunLength (const SDL_Point &inPt, int inDx, int inDy) const
{
	// guaranteed to return >= 1

	unsigned int runLength = 1;
	int value = Get (inPt);

	for (int y = inPt.y + inDy, x = inPt.x + inDx; y >= 0 && y < (int)mHeight && x >= 0 && x < (int)mWidth; y += inDy, x += inDx)
	{
		if (Get(x,y) == value)
			runLength++;
		else
			return runLength;
	}

	return runLength;
}


bool
CGameBoard::RemoveCombinations (std::list<SDL_Point> &outEntries)
{
	// first get a list of all cells that form part of one or more combinations

	int runLength = 0;

	// remove horizontal runs
	for (int y = 0; y < (int)mHeight; y++)
	{
		for (int x = 0; x < (int)mWidth - (mMinRunLength-1); x += runLength)
		{
			runLength = GetRunLength ({ x, y }, 1, 0);

			if (runLength >= mMinRunLength && Get(x,y) != sEmpty)
				for (int xx = x; xx < x + runLength; xx++)
					outEntries.push_back ({ xx, y});
		}
	}

	// remove vertical runs
	for (int x = 0; x < (int)mWidth; x++)
	{
		for (int y = 0; y < (int)mHeight - (mMinRunLength-1); y += runLength)
		{
			runLength = GetRunLength ({ x, y }, 0, 1);

			if (runLength >= mMinRunLength && Get(x,y) != sEmpty)
				for (int yy = y; yy < y + runLength; yy++)
					outEntries.push_back ({ x, yy });
		}
	}

	// second, clear all marked entries

	std::for_each (outEntries.begin (), outEntries.end (), [this] (SDL_Point inPt)
	{
		Set (inPt, sEmpty);
	});

	return !outEntries.empty();
}


////////////////////////////////////////////////////////////////////////////////////////////////////


bool
CGameBoard::GetDropInfo (std::vector<int> &outDropInfo) const
{
	// gets the lowest empty y-coordinate for each column, or -1 if column is full

	outDropInfo.resize (mWidth);

	bool hasEmpty = false;

	for (int x = 0; x < (int) mWidth; x++)
	{
		outDropInfo[x] = -1;
		for (int y = mHeight - 1; y >= 0; y--)
		{
			if (IsEmpty ({ x, y }))
			{
				outDropInfo[x] = y;
				hasEmpty = true;
				break;
			}
		}
	}

	return hasEmpty;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
