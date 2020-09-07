#pragma once

//Computing optical flow for binary masks

#include "ofMain.h"



struct ofxKuRasterOptFlow {
	ofxKuRasterOptFlow();
	void setup(int w, int h);   //320,180
	void update(const vector<unsigned char> &mask, int w, int h); //mask will be resized to optical flow size

	vector<float> &flowX() { return flowX_; }
	vector<float> &flowY() { return flowY_; }
	vector<float> &flowZ() { return flowZ_; }	//experimental
	
	void draw(float x, float y, float w, float h, float amp, int step);	//sets colors
	void resizeFlowTo(int outW, int outH, vector<float> &outX, vector<float> &outY);

	//params
	int w() { return w_; }			//flow size
	int h() { return h_; }
	int blur_input;		//input blurring (3)
	int blur_data;		//data blurring - for computing mean value (7)
	float eps_count;	//percent of pixels in area to use this for optical flow computing (0.05)

	//vector<float> mask_internal();	//latest internal mask

private:
	int w_, h_;
	int enl_;	//enlarge
	vector<float> flowX0_, flowY0_, flowX_, flowY_, flowZ_;

	vector<float> mask1_, mask2_;
	vector<float> mask1Z_, mask2Z_;

	vector<float> mask_temp0_, mask_temp_;

	template <typename T>
	void raster_enlarge(vector<T> &in, int w, int h, int enlarge, vector<T> &out, int &w1, int &h1) {
		w1 = w + 2 * enlarge;
		h1 = h + 2 * enlarge;
		out.resize(w1*h1);	//TODO we not clearing it, so if changed sized realtime, will be junk at borders
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				out[x + enlarge + w1 * (y + enlarge)] = in[x + w * y];
			}
		}
	}

	template <typename T>
	void raster_crop(vector<T> &in, int w, int h, int enlarge, vector<T> &out, int &w1, int &h1) {
		w1 = w - 2 * enlarge;
		h1 = h - 2 * enlarge;
		out.resize(w1*h1);	
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				out[x + w1 * y] = in[x + enlarge + w * (y + enlarge)];
			}
		}
	}
};


