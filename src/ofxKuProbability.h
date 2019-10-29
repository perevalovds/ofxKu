#pragma once

//Generator of values with given distribution

#include "ofMain.h"


struct ofxKuProbability {
	ofxKuProbability();

	//Setup by adding pairs (value, weight), weights: weight>0, and are relative, not expected their sum is 1
	void add_value(float value, float weight);
	float generate_value();	//generate from pairs


	//---------------------------------------------
	//Setup from density array, density can be unnormalized
	void setup(const vector<float> &density);

	//Setup from file, density can be unnormalized
	//File consists of lines with density numbers:
	//0.001
	//0.2
	//...
	void setup(string density_file);
	float generate(float range0, float range1);
	int generate_index();



private:
	//pairs (weight, value)
	bool use_pairs = false;
	bool pairs_dirty = true;
	vector<float> values_;
	vector<float> weights_;

	//just probability
	vector<float> prob_;
	int n_;

};


