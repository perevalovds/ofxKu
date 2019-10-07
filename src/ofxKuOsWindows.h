#pragma once

//Here is the following Windows-specific functions:
//- controlling OpenGL windows (make borderless, set position and size for borderless fullscreen) 
//- console window(hide, show),
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


};


//Screen grabber
struct ofxKuOsWindowsScreenGrabber {
	bool setup(int w, int h);
	bool grab(int x, int y);

	int w = 0;
	int h = 0;

	ofPixels pixels;
};

