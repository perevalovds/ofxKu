#include "ofxKuOsWindows.h"


//--------------------------------------------------------------
//Functions for borderless fullscreen emulation

//Make current oF window borderless
void ofxKuOsWindows::SetMainWindowBorderless() {	
	HWND hw = WindowFromDC(wglGetCurrentDC());

	if (hw) {
		SetWindowLong(hw, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}
	//restore window style
	//SetWindowLong( hw,GWL_STYLE, WS_POPUP|WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_BORDER );
	//SetActiveWindow(hw);

}
//--------------------------------------------------------------
//Set size fo position of the current oF window 
void ofxKuOsWindows::SetMainWindowPositionSize(int x, int y, int w, int h) {
	HWND hw = WindowFromDC(wglGetCurrentDC());

	if (hw) {
		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
		SetWindowPos(hw, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
	}
}

//--------------------------------------------------------------
//Show and hide console window
void ofxKuOsWindows::ShowConsole() {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

//--------------------------------------------------------------
void ofxKuOsWindows::HideConsole() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

//--------------------------------------------------------------
