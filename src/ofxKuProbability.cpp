#include "ofxKuProbability.h"
#include "ofxKuFile.h"

//--------------------------------------------------------------
ofxKuProbability::ofxKuProbability() {
	n_ = 0;
}

//--------------------------------------------------------------
//Setup by adding pairs (weight, value), weights: weight>0, and are relative, not expected their sum is 1
void ofxKuProbability::add_value(float value, float weight) {
	use_pairs = true;
	pairs_dirty = true;
	values_.push_back(value);
	weights_.push_back(weight);
}

//--------------------------------------------------------------
float ofxKuProbability::generate_value() {	//generate from pairs
	if (pairs_dirty) {
		setup(weights_);
		pairs_dirty = false;
	}
	int i = generate_index();
	if (i >= 0 && i < values_.size()) {
		return values_[i];
	}
	return 0;	//TODO warning
}


//--------------------------------------------------------------
//Setup from density array, density can be unnormalized
void ofxKuProbability::setup(const vector<float> &density) {
	prob_ = density;
	n_ = prob_.size();
	double sum = 0;
	for (int i = 0; i<n_; i++) {
		sum += prob_[i];
	}
	if (sum > 0) {
		for (int i = 0; i<n_; i++) {
			prob_[i] /= sum;
		}
	}
}

//--------------------------------------------------------------
void ofxKuProbability::setup(string density_file) {
	vector<string> file = ofxKuFileReadStrings(density_file);
	int n = file.size();
	vector<float> v(n);
	for (int i = 0; i<n; i++) {
		v[i] = max(ofToFloat(file[i]), 0.0f);
	}
	setup(v);
}

//--------------------------------------------------------------
float ofxKuProbability::generate(float range0, float range1) {
	int I = generate_index();
	return ofMap(I, 0, n_ - 1, range0, range1);
}

//--------------------------------------------------------------
int ofxKuProbability::generate_index() {
	if (n_ <= 1) return 0;
	float p = ofRandom(0, 1);
	float sum = 0;
	int n = prob_.size();
	int I = 0;
	for (int i = 0; i < n; i++) {
		sum += prob_[i];
		if (p <= sum) {
			I = i;
			break;
		}
	}
	return I;
}

//--------------------------------------------------------------
