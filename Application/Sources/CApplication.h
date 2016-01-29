// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

////////////////////////////////////////////////////////////////////////////////////////////////////


class CApplication
{
public:
								CApplication (const std::string inAppTitle);
								~CApplication (void);

	void						Initialize (void);
	void						Finalize (void);
	void						Run (void);

	void						RequestQuit (void);

private:

	mutable bool				mQuitRequested;
	
	std::string					mAppTitle;
	std::string					mAppPath;

	CFontPtr					mDefaultFont;
	CStagePtr					mStage;
	CGamePtr					mGame;

	const int					mWindowWidth;
	const int					mWindowHeight;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CAPPLICATION_H__
