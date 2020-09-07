#include "ofxKuRasterOptFlow.h"
#include "ofxKuRaster.h"
#include "ofxKuDrawRasters.h"

//-------------------------------------------------------------------------------
ofxKuRasterOptFlow::ofxKuRasterOptFlow() {
	w_ = 100;
	h_ = 100;
	blur_input = 3;		//Parameter may change online
	blur_data = 7;      //Parameter may change online
	enl_ = 7;

	eps_count = 0.05;
}

//-------------------------------------------------------------------------------
void ofxKuRasterOptFlow::setup(int w, int h) {
	mask1_.clear();
	mask2_.clear();

	w_ = w;
	h_ = h;
	flowX_.resize(w_*h_);
	flowY_.resize(w_*h_);
	flowZ_.resize(w_*h_);
	fill(flowX_.begin(), flowX_.end(), 0);
	fill(flowY_.begin(), flowY_.end(), 0);
	fill(flowZ_.begin(), flowZ_.end(), 0);

}

//-------------------------------------------------------------------------------
void ofxKuRasterOptFlow::update( const vector<unsigned char> &mask0, int w0, int h0) {
	int enlarge = max(blur_input, blur_data);
	if (enlarge != enl_) {
		mask2_.clear();
	}
	enl_ = enlarge;

	ofxKuRasterResize_nearest(mask0, w0, h0, mask_temp0_, w_, h_);
	int w, h;
	raster_enlarge(mask_temp0_, w_, h_, enl_, mask_temp_, w, h);

	//previous frame
	if (mask2_.size() == w*h) {
		mask1Z_ = mask2Z_;
		mask1_ = mask2_;
	}
	
	//resize, threshold and blur input mask	
	ofxKuRasterBlur_float(mask_temp_,w, h, blur_input,mask2Z_);				//blurred version for Z-optical flow	
	ofxKuRasterThreshold(mask_temp_, mask_temp_, w, h, 0.0f, 0.0f, 1.0f);
	ofxKuRasterBlur_float(mask_temp_,w,h,blur_input,mask2_);				//blurred thresholded for 2D binary optical flow

	if (mask1_.size()!=w*h || mask2_.size()!=w*h) {
		//wait next frame
		return;
	}

	//moves
	vector<float> X( w * h );
	vector<float> Y( w * h );
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			X[ x + w * y ] = x;
			Y[ x + w * y ] = y;
		}
	}

	//multiply
	vector<float> X1,Y1,X2,Y2;
	ofxKuRasterMultiply(X, mask1_, X1, w, h);		//TODO can use from previous frame!
	ofxKuRasterMultiply(Y, mask1_, Y1, w, h);
	ofxKuRasterMultiply(X, mask2_, X2, w, h);
	ofxKuRasterMultiply(Y, mask2_, Y2, w, h);

	//blur
	vector<float> X1b,Y1b,X2b,Y2b,mask1b,mask2b;
	ofxKuRasterBlur_float(X1,w,h,blur_data,X1b);
	ofxKuRasterBlur_float(Y1,w,h,blur_data,Y1b);
	ofxKuRasterBlur_float(X2,w,h,blur_data,X2b);
	ofxKuRasterBlur_float(Y2,w,h,blur_data,Y2b);
	ofxKuRasterBlur_float(mask1_,w,h,blur_data,mask1b);
	ofxKuRasterBlur_float(mask2_,w,h,blur_data,mask2b);

	//clean bad zones
	//vector<int> zone(w_ * h_,1);
	for (int i=0; i<w*h; i++) {
		if (mask1b[i] <= eps_count || mask2b[i] < eps_count) {
			X1b[i] = Y1b[i] = X2b[i] = Y2b[i] = 0;
			//zone[i] = 0;
		}
	}

	//divide
	ofxKuRasterDivide(X1b,mask1b,X1,w,h,eps_count,0.0f);	//TODO can here already crop...
	ofxKuRasterDivide(Y1b,mask1b,Y1,w,h,eps_count,0.0f);
	ofxKuRasterDivide(X2b,mask2b,X2,w,h,eps_count,0.0f);
	ofxKuRasterDivide(Y2b,mask2b,Y2,w,h,eps_count,0.0f);

	//find 2D binary mask flow
	ofxKuRasterSubtract(X2,X1,flowX0_,w,h);
	ofxKuRasterSubtract(Y2,Y1,flowY0_,w,h);
	int w1, h1;
	raster_crop(flowX0_, w, h, enl_, flowX_, w1, h1);
	raster_crop(flowY0_, w, h, enl_, flowY_, w1, h1);

	//TODO check w1 == w_, h1 == h_

	//let's find flow at Z direction (experimental, works poor inside contour)
	/*float eps_countZ = 0.3;
	for (int y=0; y<h_; y++) {
		for (int x=0; x<w_; x++) {
			int i = x + w_*y;
			flowZ_[i] = 0;
			if (mask1b[i] > eps_countZ) {
				int x2 = x + int(flowX_[i]);
				int y2 = y + int(flowY_[i]);
				int i2 = x2 + w_*y2;
				if (x2 >= 0 && x2 < w_ && y2 >= 0 && y2 < h_ && mask2b[i2] > eps_countZ) {
					flowZ_[i] = mask2Z_[i2] - mask1Z_[i];	//flowZ_[i2]!
				}
			}
		}
	}*/

}

//-------------------------------------------------------------------------------
void ofxKuRasterOptFlow::draw(float x0, float y0, float w0, float h0, 
							  float amp, int step) {
	//ofSetColor(255);
	//ofxKuDrawRaster(mask2_, w_, h_, x0, y0, w0, h0, 0, 255);
	//ofxKuDrawRaster(flowX_, w_, h_, x0, y0, w0, h0, -10, 10);

	if (w_ > 0 && h_ > 0 && flowX_.size() == w_ * h_ && flowY_.size() == w_ * h_) {
		ofPushMatrix();
		ofTranslate(x0, y0);
		ofScale(w0 / w_, h0 / h_);		
		//2D
		ofSetColor(255,0,0);
		for (int y=0; y<h_; y+=step) {
			for (int x=0; x<w_; x+=step) {
				int i = x + w_*y;
				if (fabs(flowX_[i])>0 && fabs(flowY_[i])>0) {
					ofLine(x, y, x+flowX_[i]*amp, y+flowY_[i]*amp);
				}
			}
		}
		//Z
		/*float ampZ = amp*0.02;
		ofFill();
		for (int y=0; y<h_; y+=step) {
			for (int x=0; x<w_; x+=step) {
				int i = x + w_*y;
				float v = fabs(flowZ_[i]);
				if (v>0) {
					float a = ofMap(v,0,10,32,255,true);
					if (flowZ_[i]>0) {
						ofSetColor(0,255,0,a);
					}
					else {
						ofSetColor(0,0,255,a);
					}
					ofRect(x-0.5,y-0.5,1,1);
					//ofLine(x, y, x-flowZ_[i]*ampZ, y+flowZ_[i]*ampZ);
				}				
			}
		}*/
		ofPopMatrix();
	}

}

//-------------------------------------------------------------------------------
void ofxKuRasterOptFlow::resizeFlowTo(int outW, int outH, vector<float> &outX, vector<float> &outY) {
	//use "maximal" length when choosing from several optical flow vectors
}

//-------------------------------------------------------------------------------

