#pragma once

//Here is Windows-specific functions for controlling 
//OpenGL windows (make borderless, set position and size for borderless fullscreen) 
//and console window(hide, show).

#include "ofMain.h"

struct ofxKuOsWindows {
	//Functions for borderless fullscreen emulation

	//Make current oF window borderless
	static void SetMainWindowBorderless();	

	//Set size fo position of the current oF window
	//(works more stable that oF implementation
	static void SetMainWindowPositionSize(int x, int y, int w, int h);

	//Show and hide console window
	static void ShowConsole();
	static void HideConsole();


};

