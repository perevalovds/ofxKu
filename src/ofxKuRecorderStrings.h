#pragma once

#include "ofMain.h"

//ofxKuRecorderStrings - class for recording and reading data into text file, one frame is held in one string
//
//-------------- USAGE ------------------
//Recording:
//    Declare RECORDER as a global variable, and use such code for adding keys to current frame
//    if (RECORDER.recording()) {
//        RECORDER.add_data("time", ofToString(time));	
//        RECORDER.add_data("dt", ofToString(dt));	
//        RECORDER.add_data("pos", ofToString(position, " "));	
//    }
//    Finally, in the end of update store collected frame:
//    if (RECORDER.recording()) {
//        RECORDER.next_frame();
//    }
//Playing:
//    Similarly, in any place of the code read values:
//    if (RECORDER.playing()) {
//        time = ofToFloat(RECORDER.get_data("time"));
//        dt = ofToFloat(RECORDER.get_data("dt"));
//        position = ofToPoint(RECORDER.get_data("pos"), " ");
//    }
//    And in the end of update() switch to the next frame: 
//    if (RECORDER.playing()) {
//	    RECORDER.next_frame();
//    }
//---------------------------------------------------------

class ofxKuRecorderStrings {
public:
	ofxKuRecorderStrings() {
		separator = ";";
	}

	void record_toggle();
	void play_toggle();

	bool save(string file_name);
	bool load(string file_name);

	bool playing() { return playing_; }
	bool recording() { return recording_; }
	int frame() { return frame_; }
	int frames() { return file.size(); }

	//Add data to the current frame by key
	//for example, key="matrix", key="bubble"
	//for separating data, used separator variable, so records in a freame are: key,value,key,value,...
	void add_data(const string &key, const string &value);

	//Get data from the current frame
	string get_data(const string &key);

	//Switch to the next frame in recording/playing modes
	void next_frame();

	//Go to a frame, used only in player mode
	void set_frame(int frame);		


	vector<string> file;

	string separator;	//separator for separating keys inside one frame (one frame is one string) 

	bool recording_ = false;
	bool playing_ = false;

	int frame_ = 0;
	string frame_data_;

protected:
	void read_data();	//read data for current frame
};


