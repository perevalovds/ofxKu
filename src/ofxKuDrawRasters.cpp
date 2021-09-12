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
//Sets drawing matrix for drawing aligned images, fit in a given rectangle; image can be scaled using "scale"
//"align" = 0 - center, -1 - top/left, 1 - right/bottom
//usage:
//	ofPushMatrix(); 
//	ofxKuDrawImageSetMatrixFit(...);
//	tex.draw(0,0); 
//	ofPopMatrix();


void ofxKuDrawImageSetMatrixFit(float img_w, float img_h, ofRectangle rect_to_fit, int align_x, int align_y, float scale) {
	float X = rect_to_fit.getLeft();
	float Y = rect_to_fit.getTop();
		
	float W = rect_to_fit.getWidth();
	float H = rect_to_fit.getHeight();

	if (W <= 0 || H <= 0 || img_w <= 0 || img_h <= 0) {
		cout << "ofxKuDrawImagePushMatrixFit - empty image or negative params" << endl;
		return;
	}

	//fit size
	float scl = min(W / img_w, H / img_h) * scale;
	float resw = img_w * scl;
	float resh = img_h * scl;
	float x = 0;	//left
	float y = 0;	//top
	if (align_x == 0) {
		x = (W - resw) / 2;
	}
	if (align_x == 1) {
		x = W - resw;
	}
	if (align_y == 0) {
		y = (H - resh) / 2;
	}
	if (align_y == 1) {
		y = H - resh;
	}
	ofTranslate(X + x, Y + y);		//<------ Translating
	ofScale(scl, scl);

}

//--------------------------------------------------------------
