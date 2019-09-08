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
MLOG("Success");		//simplest with settings color color
MLOG("Error", ofColor(255,0,0)); //message with custom color
//Message with one text on the screen and another text printed to console
MLOG("Задача завершена", "Task is completed", ofColor(255,0,0));

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

struct ofxKuMessageLogBigMessage {
	string message, message_console;	//supported multiline messages, separated by "\n"
	ofRectangle rect = ofRectangle(0,0,200,200);
	ofColor color_font = ofColor(255, 255, 0);
	ofColor color_back = ofColor(64);
	float duration_sec = 3;
	float font_size = 20;

	void start();
	void draw(ofxFontStash &font);
	//------------------------
	float time_end_ = -1000;
};

struct ofxKuMessageLog {
	void setup(string font_file);
	void set_parameters(const ofxKuMessageLogParams &params);
	

	void clear();
	void log(const string &s);	//add new message to log
	void log(const string &s, const ofColor &color);
	void log(const string &s, const string &s_console);	//different messages on the screen and console
	void log(const string &s, const string &s_console, const ofColor &color);
	
	//show "big message" for several seconds, used for showing warnings in the center of screen
	//supported multiline messages, separated by "\n"
	void big_message(ofxKuMessageLogBigMessage &big);
	
	void draw();

	ofxFontStash font;
	ofxKuMessageLogParams params;
	vector<string> lines;
	vector<ofColor> colors;

	//big message
	ofxKuMessageLogBigMessage big_;
};


extern ofxKuMessageLog MLOGGER;

void MLOG(const string &s);	//add new message to log
void MLOG(const string &s, const ofColor &color);
void MLOG(const string &s, const string &s_console);
void MLOG(const string &s, const string &s_console, const ofColor &color);

//also see MLOGGER.big_message() for "big message" 
