#pragma once

//Converting data types

#include "ofMain.h"

//Convert vector<string> to vector<int>, vector<float> and back

template<class T>
std::vector<std::string> ofToVectorString(const std::vector<T>& values) {
	int n = values.size();
	std::vector<std::string> out(n);
	for (int i = 0; i < n; i++) {
		out[i] = ofToString(values[i]);
	}
	return out;
}

std::vector<int> ofToVectorInt(const std::vector<string>& values);
std::vector<float> ofToVectorFloat(const std::vector<string>& values);

//Convert ofPoint to string and back
std::string ofToString(const ofPoint &p, const std::string &separator);
ofPoint ofToPoint(const std::string &s, const std::string &separator);


//Convert matrix to string and back
string ofToString(const ofMatrix4x4 &matrix, string separator);
ofMatrix4x4 ofToMatrix(const string &s, string separator);


