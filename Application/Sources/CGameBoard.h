// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 12-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CGAMEBOARD_H__
#define __CGAMEBOARD_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CDrawable.h"
#include "CMatrix.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This class stores the game board logic
//
//	It keeps track of the current situation and supplies methods to test and apply moves
//	Artifacts are stored as an int, which can be seen as the index in the artifact list
//	Empty cells are represented by -1


class CGameBoard :
	public CMatrix<int>
{ 
public:
								CGameBoard (unsigned int inColCount, unsigned int inRowCount, unsigned int inTypeCount);
	virtual						~CGameBoard (void);
	
	void						Fill (void);			// fill with random artifacts
	void						Clear (void);			// remove all artifacts

	void						Swap (const SDL_Point &inPt1, const SDL_Point &inPt2);

//	bool						CanSwap (const SDL_Point &inPt1, const SDL_Point &inPt2);
//	bool						HasCombination (const SDL_Point &inPt);

	bool						RemoveCombinations (std::list<SDL_Point> &outEntries);
	int							GetRunLength (const SDL_Point &inPt, int inDx, int Dy) const;

	bool						GetDropInfo (std::vector<int> &outDropInfo) const; // gets the lowest empty y-coordinate for each column, or -1 if column is full

	bool						IsEmpty (const SDL_Point &inPt) const;
	int							SetRandom (const SDL_Point &inPt);

private:

	int							GetRandomType (void);

	const unsigned int			mTypeCount;		// number of different artifact types

	static const int			sEmpty;
	
	const int					mMinRunLength;

	std::default_random_engine				mRandomGenerator;
	std::uniform_int_distribution<int>		mTypesDistribution;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CGAMEBOARD_H__
