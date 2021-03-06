#pragma once

//Raster functions

#include "ofMain.h"
#include "ofxKuRasterArithmetics.h"
#include "ofxKuRasterBlur.h"
#include "ofxKuRasterInterpolate.h"
#include "ofxKuRasterMorphology.h"
#include "ofxKuRasterOptFlow.h"
#include "ofxKuRasterResize.h"
#include "ofxKuFloodFill.h"

//RGB8 pixel, actually as ofColor
struct ofxKuRasterPixelRGB8 {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	ofxKuRasterPixelRGB8() {}
	ofxKuRasterPixelRGB8(unsigned char v) { r = g = b = v; }
	ofxKuRasterPixelRGB8(unsigned char r0, unsigned char g0, unsigned char b0) { 
		r = r0;
		g = g0;
		b = b0;
	}
	ofxKuRasterPixelRGB8(const ofColor &c) {
		r = c.r;
		g = c.g;
		b = c.b;

	}

};

//RGB8 raster
struct ofxKuRasterRGB8 {
	int w = 0;
	int h = 0;
	vector<ofxKuRasterPixelRGB8> pixels;

	unsigned char *pixels_pointer() { return (unsigned char *)&pixels[0]; }

	void allocate(int w0, int h0) {
		w = w0;
		h = h0;
		pixels.resize(w*h);
	}
	void clear() {
		w = h = 0;
		pixels.resize(0);	//TODO not freeing real memory; to achieve it, we need to use vector<...> *pixels;
	}
	bool empty() {
		return w == 0 && h == 0;
	}

	void fill(ofxKuRasterPixelRGB8 value = ofxKuRasterPixelRGB8(0)) {
		for (int i = 0; i < w*h; i++) {
			pixels[i] = value;
		}
	}

};


