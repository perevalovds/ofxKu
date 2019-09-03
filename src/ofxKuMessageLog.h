#pragma once

//Messages on the screen
//Required ofxFontStash addon for output on the screen

#include "ofMain.h"
#include "ofxFontStash.h"

struct ofxKuMessageLogParams {
	ofColor color;
	float size = 30;
	ofPoint pos = ofPoint(300, 20);

	int capacity = 30;
};

struct ofxKuMessageLog {
	void setup(string font_file);
	void set_parameters(const ofxKuMessageLogParams &params);
	

	void clear();
	void log(const string &s);	//add new message to log
	void draw();

	ofxFontStash font;
	ofxKuMessageLogParams params;
	vector<string> lines;

};


extern ofxKuMessageLog MLOG;