#include "ofxKuRasterArithmetics.h"

//--------------------------------------------------------------
void ofxKuRasterHistogram(const vector<unsigned char> &input, vector<int> &histogram, int w, int h) {
	histogram.resize(256);
	fill(histogram.begin(),histogram.end(),0);
	for (int i=0; i<w*h; i++) {
		histogram[input[i]]++;
	}
}

//--------------------------------------------------------------
