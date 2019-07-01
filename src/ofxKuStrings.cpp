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
std::string ofToString(const ofPoint &p, const std::string &separator) {
	return ofToString(p.x) + separator + ofToString(p.y) + separator + ofToString(p.z);
}

//--------------------------------------------------------------
ofPoint ofToPoint(const std::string &s, const std::string &separator) {
	vector<float> v = ofToVectorFloat(ofSplitString(s, separator));
	ofPoint p;
	if (v.size() >= 2) {
		p.x = v[0];
		p.y = v[1];
	}
	if (v.size() >= 3) {
		p.z = v[2];
	}
	return p;
}

//--------------------------------------------------------------
string ofToString(const ofMatrix4x4 &matrix, string separator) {
	int n = 16;
	vector<float> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = matrix.getPtr()[i];
	}
	return ofJoinString(ofToVectorString(v), separator);
}


//--------------------------------------------------------------
ofMatrix4x4 ofToMatrix(const string &s, string separator) {
	ofMatrix4x4 matrix;
	vector<float> v = ofToVectorFloat(ofSplitString(s, separator));
	if (v.size() >= 16) {
		matrix.set(&v[0]);
	}
	return matrix;
}
//--------------------------------------------------------------
