#pragma once

//Here is the following Windows-specific functions:
//- controlling OpenGL windows (make borderless, set position and size for borderless fullscreen) 
//- console window(hide, show),
//- mouse move and click emulation
//- sending keyboard hit to a window, found by window class name(see windows / winlister for getting window class).
//- screen grabber (GDI method)

#include "ofMain.h"

struct ofxKuOsWindows {
	//Functions for borderless fullscreen emulation

	//Make current oF window borderless
	static void SetMainWindowBorderless();

	//Restore current oF window border
	//NOTE: currently it works bad in oF, because y-coord of mouse is shifted
	static void RestoreMainWindowBorder();

	//Set size and position of the current oF window
	//(works more stable that oF implementation
	static void SetMainWindowPositionSize(int x, int y, int w, int h);


	//Show and hide console window
	static void ShowConsole();
	static void HideConsole();

	//Mouse move and click emulation
	static void MouseExec(int screen_x, int screen_y, bool leftClick, bool rightClick);

	//Send keyboard to a window with given class name
	//To search windows class use WinLister in 'windows/winlister' folder of the addon
	static bool SendKeyboard_by_class(string window_title, int key);
};


//Screen grabber
//TODO release after usage hdcScr, hdcMem, hbmScr, see CPP code
struct ofxKuOsWindowsScreenGrabber {
	bool grab(int x, int y, int w, int h);
	ofPixels pixels;
};

