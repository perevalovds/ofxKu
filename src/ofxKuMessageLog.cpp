#include "ofxKuMessageLog.h"

ofxKuMessageLog MLOGGER;

//--------------------------------------------------------------
void MLOG(const string &s) {	//add new message to log
	MLOGGER.log(s);
}

//--------------------------------------------------------------
void MLOG(const string &s, const ofColor &color) {
	MLOGGER.log(s, color);
}

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
	colors.clear();
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s) {
	log(s, params.color);
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s, const ofColor &color) {
	cout << s << endl;
	lines.push_back(s);
	colors.push_back(color);
	if (lines.size() > params.capacity) {
		lines.erase(lines.begin());
		colors.erase(colors.begin());
	}
}

//--------------------------------------------------------------
void ofxKuMessageLog::draw() {
	//ofSetColor(params.color);
	//font.draw("Три", 30, x, y);
	ofFill();	//required for using font stash
	ofPoint &pos = params.pos;
	for (int i = 0; i < lines.size(); i++) {
		ofSetColor(colors[i]);
		font.draw(lines[i], params.font_size, pos.x, pos.y + i * params.font_size);
	}
	//string s = ofJoinString(lines, "\n");
	//font.drawMultiLine(s, params.size, params.pos.x, params.pos.y);
}

//--------------------------------------------------------------
