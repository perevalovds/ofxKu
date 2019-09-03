#include "ofxKuMessageLog.h"

MessageLog MLOG;

//--------------------------------------------------------------
void ofxKuMessageLog::setup(string font_file) {
	font.setup(font_file, 1.0, 1024, false, 8, 1.5); //1.0, 512, false); // , 8, 1.5);
	font.setCharacterSpacing(0);
}

//--------------------------------------------------------------
void ofxKuMessageLog::set_parameters(const ofxKuMessageLogParams &params) {
	this->params = params;
}

//--------------------------------------------------------------
void ofxKuMessageLog::clear() {
	lines.clear();
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s) {
	cout << s << endl;
	lines.push_back(s);
	if (lines.size() > params.capacity) {
		lines.erase(lines.begin());
	}
}

//--------------------------------------------------------------
void ofxKuMessageLog::draw() {
	//font.draw("Три", 30, x, y);
	ofSetColor(params.color);
	ofFill();
	string s = ofJoinString(lines, "\n");
	font.drawMultiLine(s, params.size, params.pos.x, params.pos.y);
}

//--------------------------------------------------------------
