#pragma once

#include "ofMain.h"

//Drawing rasters

void ofxKuDrawRaster(vector<unsigned char> &pix, int w, int h, float X, float Y, float W, float H, bool nearest_filter = true, bool rgb = false);
void ofxKuDrawRaster(vector<float> &pix, int w, int h, float X, float Y, float W, float H, float val0=0, float val1=255,
	bool nearest_filter = true);

void ofxKuOfGrayPixelsToRaster(ofPixels &pixels, vector<unsigned char> &pix, int &w, int &h);

//ofPushMatrix for drawing aligned images, fit in a given rectangle; image can be scaled using "scale"
//"align" = 0 - center, -1 - top/left, 1 - right/bottom
//usage:
//	ofPushMatrix(); 
//	ofxKuDrawImageSetMatrixFit(...);
//	tex.draw(0,0); 
//	ofPopMatrix();
void ofxKuDrawImageSetMatrixFit(float img_w, float img_h, ofRectangle rect_to_fit, int align_x = 0, int align_y = 0, float scale=1);