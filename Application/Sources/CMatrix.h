// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 15-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CMATRIX_H__
#define __CMATRIX_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	convenience class

template <typename T>
class CMatrix
{
public:
								CMatrix (void) : mWidth (0), mHeight (0) { }
								CMatrix (unsigned int inWidth, unsigned int inHeight) : mWidth (inWidth), mHeight (inHeight) { mElems.resize (mWidth * mHeight); }

								// std::vector type resize and clear function
	void						Resize (unsigned int inWidth, unsigned int inHeight) { mWidth = inWidth; mHeight = inHeight; mElems.resize (mWidth * mHeight); }
	void						Clear (void) { mWidth = 0; mHeight = 0; mElems.clear(); }

								// returns a reference to the entry, or a copy
	T &							operator () (unsigned int inX, unsigned int inY) { return mElems[inY * mWidth + inX]; }
	T							operator () (unsigned int inX, unsigned int inY) const { return mElems[inY * mWidth + inX]; }

								// returns a reference to the entry, or a copy
	T &							operator () (const SDL_Point &inPt) { return operator()(inPt.x, inPt.y); }
	T							operator () (const SDL_Point &inPt) const { return operator()(inPt.x, inPt.y); }

								// returns a copy of the entry
	T							Get (unsigned int inX, unsigned int inY) const { return operator()(inX, inY); }
	T							Get (const SDL_Point &inPt) const { return operator()(inPt); }

								// copy the value to the entry
	void						Set (unsigned int inX, unsigned int inY, T inValue) { operator()(inX, inY) = inValue; }
	void						Set (const SDL_Point &inPt, T inValue) { operator()(inPt) = inValue; }

protected:

	unsigned int				mWidth;
	unsigned int				mHeight;

private:

	std::vector<T>				mElems;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CMATRIX_H__
