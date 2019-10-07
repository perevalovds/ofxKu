#include "ofxKuOsWindows.h"

#include<iostream>
#include<windows.h>

//--------------------------------------------------------------
//Functions for borderless fullscreen emulation

//Make current oF window borderless
void ofxKuOsWindows::SetMainWindowBorderless() {	
	HWND hw = WindowFromDC(wglGetCurrentDC());

	if (hw) {
		SetWindowLong(hw, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}
}

//--------------------------------------------------------------
//Restore current oF window border
//NOTE: currently it works bad in oF, because y-coord of mouse is shifted
void ofxKuOsWindows::RestoreMainWindowBorder() {
	HWND hw = WindowFromDC(wglGetCurrentDC());

	if (hw) {
		SetWindowLong(hw, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_BORDER);
		//SetActiveWindow(hw);
	}
}

//--------------------------------------------------------------
//Set size and position of the current oF window 
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
//Screen grabber

HDC hdcScr = 0;
HDC hdcMem = 0;
HBITMAP hbmScr = 0;

//--------------------------------------------------------------
int CaptureBMP(int grabX, int grabY, int grabW, int grabH, unsigned char *outData32)
{
	// Source[1]


	BITMAP bmp;
	//int iXRes, iYRes;

	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	if (!hdcScr) {
		hdcScr = CreateDC(L"DISPLAY", NULL, NULL, NULL);		//как вариант - CreateCompatibleDC(0);
		hdcMem = CreateCompatibleDC(hdcScr);
	}

	//iXRes = GetDeviceCaps(hdcScr, HORZRES);
	//iYRes = GetDeviceCaps(hdcScr, VERTRES);

	// Create a compatible bitmap for hdcScreen.
	if (hbmScr == 0) {							//! если сменилс¤ размер, то надо пересоздать
		hbmScr = CreateCompatibleBitmap(hdcScr, grabW, grabH);
	}
	if (hbmScr == 0) {
		cout << "CaptureBMP Error 1" << endl;
		return 0;
	}

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(hdcMem, hbmScr)) {
		cout << "CaptureBMP Error 2" << endl;
		return 0;
	}

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!StretchBlt(hdcMem,
		0, 0, grabW, grabH,
		hdcScr,
		grabX, grabY, grabW, grabH,
		SRCCOPY)) {
		cout << "CaptureBMP Error 3" << endl;
		return 0;
	}


	// Source[2]
	PBITMAPINFO pbmi;
	WORD cClrBits;

	// Retrieve the bitmap's color format, width, and height.
	if (!GetObjectW(hbmScr, sizeof(BITMAP), (LPSTR)&bmp)) {
		cout << "CaptureBMP Error 4" << endl;
		return 0;
	}

	// Convert the color format to a count of bits.
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
	// data structures.)
	if (cClrBits != 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));

	// There is no RGBQUAD array for the 24-bit-per-pixel format.
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) / 8
		* pbmi->bmiHeader.biHeight * cClrBits;

	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pbmi->bmiHeader.biClrImportant = 0;

	HANDLE hf;                  // file handle
	BITMAPFILEHEADER hdr;       // bitmap file-header
	PBITMAPINFOHEADER pbih;     // bitmap info-header
	LPBYTE lpBits;              // memory pointer
	DWORD dwTotal;              // total count of bytes
	DWORD cb;                   // incremental count of bytes
	BYTE *hp;                   // byte pointer
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbmi;


	int dataSize = pbih->biSizeImage;
	if (dataSize != grabW * grabH * 4) {
		cout << "CaptureBMP ERROR Grab, bad data size " << dataSize << ", expect " << grabW * grabH * 4 << endl;
	}

	//outData32.resize(dataSize);

	lpBits = outData32; //(LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	//lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits) {
		cout << "CaptureBMP Error 5" << endl;
		return 0;
	}


	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.
	if (!GetDIBits(hdcMem, hbmScr, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
		cout << "CaptureBMP Error 6" << endl;
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------
bool ofxKuOsWindowsScreenGrabber::setup(int w, int h) {
	this->w = w;
	this->h = h;
	pixels.allocate(w, h, 4);
	return true;
}

//--------------------------------------------------------------
bool ofxKuOsWindowsScreenGrabber::grab(int x, int y) {
	int res = CaptureBMP(x, y, w, h, pixels.getPixels());

	if (res) {
		
		//flip y & R<->B
		unsigned char *data = pixels.getPixels();
		for (int y = 0; y < h/2; y++) {
			for (int x = 0; x < w; x++) {
				int i = 4 * (x + w * y);
				int j = 4 * (x + w * (h-1-y));

				swap(data[i + 0], data[j + 2]);
				swap(data[i + 1], data[j + 1]);
				swap(data[i + 2], data[j + 0]);
				swap(data[i + 3], data[j + 3]);
			}
		}
	}
	return (res);
}

//--------------------------------------------------------------
