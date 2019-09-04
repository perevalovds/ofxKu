#pragma once

//Messages on the screen
//Required ofxFontStash addon for output on the screen
//MLOGGER - global object
//MLOG - function for adding new message

/*Example usage

//Setup
ofxKuMessageLogParams prm;
prm.pos = ofPoint(20, 500);
prm.size = 25;
MLOGGER.setup("fonts/Arial Unicode.ttf");
MLOGGER.set_parameters(prm);

//Add new message
MLOG("Тест 1");
MLOG("Success", ofColor(64,255,64));

//Draw
MLOGGER.draw();
*/


#include "ofMain.h"
#include "ofxFontStash.h"

struct ofxKuMessageLogParams {
	ofColor color;
	float font_size = 30;
	ofPoint pos = ofPoint(300, 20);

	int capacity = 30;
};

struct ofxKuMessageLog {
	void setup(string font_file);
	void set_parameters(const ofxKuMessageLogParams &params);
	

	void clear();
	void log(const string &s);	//add new message to log
	void log(const string &s, const ofColor &color);	
	void draw();

	ofxFontStash font;
	ofxKuMessageLogParams params;
	vector<string> lines;
	vector<ofColor> colors;

};


extern ofxKuMessageLog MLOGGER;

void MLOG(const string &s);	//add new message to log
void MLOG(const string &s, const ofColor &color);