// (C)opyright by Oscar ter Hofstede
// All rights reserved.
//
// Created on 11-04-2014 by Oscar ter Hofstede
//
// $Revision: $
// $Date: $
// $Author: $

#ifndef __CGRAPHICS_H__
#define __CGRAPHICS_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This class contains all that is drawn on the stage, and allows updating animations

class CStage :
	public std::enable_shared_from_this<CStage>
{
public:
								CStage (const std::string inWindowTitle, unsigned int inWidth, unsigned int inHeight, CFontPtr inDefaultFont);
								~CStage (void);

	void						Finalize (void); // call this to clean up (gives the object a change to remove circular references)

	void						StartRenderTimer (unsigned int inFrameRate);
	void						StopRenderTimer (void);

	void						AddDrawable (CDrawablePtr inDrawable, unsigned int inZIndex = 1);
	void						RemoveDrawable (CDrawablePtr inDrawable);

	void						SetBackground (const std::string inFileName);

	bool						ShowHud (bool inShow = true);						// returns previous setting
	bool						ToggleHud (void) { return ShowHud (!mShowHud); }	// returns previous setting

	CTexturePtr					CreateTexture (const std::string inFileName);

	CRendererPtr				GetRenderer (void) { return mRenderer; }

								// update everything on the stage without drawing anything
	void						Update (unsigned int inCurTicks);

								// render everything on the stage and copy it to the screen
	void						Render (void);

private:

	static Uint32				RenderTimerCallback (Uint32 inInterval, void *inStageInstance);

	SDL_Window *				mMainWindow;
	CRendererPtr				mRenderer;
	SDL_TimerID					mRenderTimerID;
	unsigned int				mFrameRate;
	bool						mLaggingBehind;

	CFontPtr					mDefaultFont;
	CTexturePtr					mBackground;
	CTextControlPtr				mHud;

	bool						mShowHud;

	// drawables are put in a layer for z-ordering
	std::vector<std::list<CDrawablePtr>>		mLayers;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __CGRAPHICS_H__
