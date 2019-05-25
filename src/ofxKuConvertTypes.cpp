#include "ofxKuConvertTypes.h"


//--------------------------------------------------------------
std::vector<int> ofToVectorInt(const std::vector<string>& values) {
	int n = values.size();
	std::vector<int> out(n);
	for (int i = 0; i < n; i++) {
		out[i] = ofToInt(values[i]);
	}
	return out;
}

//--------------------------------------------------------------
std::vector<float> ofToVectorFloat(const std::vector<string>& values) {
	int n = values.size();
	std::vector<float> out(n);
	for (int i = 0; i < n; i++) {
		out[i] = ofToFloat(values[i]);
	}
	return out;
}

//--------------------------------------------------------------
string ofxKuMatrixToString(const ofMatrix4x4 &matrix, string separator) {
	int n = 16;
	vector<float> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = matrix.getPtr()[i];
	}
	return ofJoinString(ofToVectorString(v), separator);
}


//--------------------------------------------------------------
ofMatrix4x4 ofxKuStringToMatrix(const string &s, string separator) {
	ofMatrix4x4 matrix;
	vector<float> v = ofToVectorFloat(ofSplitString(s, separator));
	if (v.size() >= 16) {
		matrix.set(&v[0]);
	}
	return matrix;
}
//--------------------------------------------------------------
