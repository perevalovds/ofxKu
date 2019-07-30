#include "ofxKuStrings.h"


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
//Advanced ofSplitString, which allows to use many one-char delimiters
//For example, this function allows to parse effectively "a(b,c)" with delimiters "(,)"
vector<string> ofSplitString_many(const string &source, const string &delimiters, bool ignoreEmpty, bool trim) {
	vector<string> result;
	if (delimiters.empty()) {
		result.push_back(source);
		return result;
	}

	string::const_iterator substart = source.begin();
	string::const_iterator subend = substart;
	while (true) {
		if (subend == source.end() || (subend != source.end() && delimiters.find(subend[0]) != string::npos)) {
			string sub(substart, subend);
			if (trim) {
				sub = ofTrim(sub);
			}
			if (!ignoreEmpty || !sub.empty()) {
				result.push_back(sub);
			}
			if (subend == source.end()) {
				break;
			}
			substart = subend + 1;
			subend = substart;
		}
		else {
			subend++;
		}
	}
	return result;
}

//--------------------------------------------------------------
//Is a string started with a given string
bool ofStringStartsWith(const string &S, const string &prefix) {
	return S.rfind(prefix, 0) == 0;
}

//--------------------------------------------------------------
//Is a string started with a any of given strings
bool ofStringStartsWith(const string &S, const vector<string> &prefix) {
	for (auto &pref : prefix) {
		if (ofStringStartsWith(S, pref)) return true;
	}
	return false;
}

//--------------------------------------------------------------
//Is a string ended with a given string
bool ofStringEndsWith(const string &S, const string &suffix) {
	return (S.length() >= suffix.length() && S.substr(S.length() - suffix.length()) == suffix);
}

//--------------------------------------------------------------
