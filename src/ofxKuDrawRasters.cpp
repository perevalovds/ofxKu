#include "ofxKuDrawRasters.h"

//--------------------------------------------------------------
void ofxKuDrawRaster(vector<unsigned char> &pix, int w, int h, float X, float Y, float W, float H, bool nearest_filter, bool rgb) {
	if (pix.size() != w * h * ((rgb) ? 3 : 1) || w * h <= 0) {
		cout << "ofxKuDrawRaster error, bad pix size" << endl;
		return;
	}
	ofTexture texture;
	texture.loadData(&pix[0],w,h,(rgb)?GL_RGB:GL_LUMINANCE);
	if (nearest_filter) {
		texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	}
	texture.draw(X,Y,W,H);
}

//--------------------------------------------------------------
void ofxKuDrawRaster(vector<float> &pix, int w, int h, float X, float Y, float W, float H, float val0, float val1,
	bool nearest_filter) {
	if (pix.size() != w*h || w*h <= 0) return;
	vector<unsigned char> pix1(w*h);
	for (int i=0; i<w*h; i++) {
		pix1[i] = ofMap(pix[i],val0,val1,0,255,true);
	}
	ofxKuDrawRaster(pix1, w, h, X, Y, W, H, nearest_filter);
}

//--------------------------------------------------------------
void ofxKuOfGrayPixelsToRaster(ofPixels &pixels, vector<unsigned char> &pix, int &w, int &h) {
	w = pixels.getWidth();
	h = pixels.getHeight();
	pix.resize(w*h);
	unsigned char *data = pixels.getPixels();
	for (int i=0; i<w*h; i++) {
		pix[i] = data[i];
	}
}

//--------------------------------------------------------------
