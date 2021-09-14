#pragma once

/*Messages on the screen
Required ofxFontStash addon for output on the screen
MLOGGER - global object
MLOG - function for adding new message

IMPORTANT NOTE: To see non-latin symbols, do the following:
1) Convert your ofApp and other h/cpp source files to UTF8 (in Windows - use Notepad++),
2) If you are using Visual Studio, set Project Settings - General - Character Set to "Use Unicode Character Set"

Example of usage:

//Setup
MLOGGER.setup("fonts/Arial Unicode.ttf");
ofxKuMessageLogParams prm;
prm.color = ofColor(140);
MLOGGER.set_parameters(prm);

//Add new message
MLOG("Success");		//simplest with settings color color
MLOG("Error", ofColor(255,0,0)); //message with custom color
//Message with one text on the screen and another text printed to console
MLOG("Задача завершена", "Task is completed", ofColor(255,0,0));

//Draw
float font_size = 25;
MLOGGER.draw(20, 20, ofGetHeight()-40, font_size);
*/


#include "ofMain.h"
#include "ofxFontStash.h"

struct ofxKuMessageLogParams {
	ofColor color;
	float font_size = 30;
	ofPoint pos = ofPoint(300, 20);

	int capacity = 30;

	void fit_capacity(float h, float font_size) {
		capacity = int(h / font_size);
	}
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
	void disable() { disabled_ = true; }	//if disabled, the all calls ignored - used for windowless mode
	bool disabled() { return disabled_; }

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
	
	//use it then you don't want to care of pos and capacity, and it will be computed automatically
	void draw(float x, float y, float h, float font_size = 20, bool use_system_font = false);

	//DEPRECATED
	//use it when you have desired pos and capacity and font_size and set it in set_parameters
	void draw(bool use_system_font = false);


	ofxFontStash font;
	ofxKuMessageLogParams params;
	vector<string> lines;
	vector<ofColor> colors;

	//big message
	ofxKuMessageLogBigMessage big_;


protected:
	void remove_old_lines();	//remove lines is lines.size exceeds capacity

	bool disabled_ = false;
};


extern ofxKuMessageLog MLOGGER;

void MLOG(const string &s);	//add new message to log
void MLOG(const string &s, const ofColor &color);
void MLOG(const string &s, const string &s_console);
void MLOG(const string &s, const string &s_console, const ofColor &color);

//also see MLOGGER.big_message() for "big message" 
