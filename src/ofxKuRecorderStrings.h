#pragma once

#include "ofMain.h"

//Recording and reading data into text file
//One frame is one string

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

	//adds data to frame (one frame is one string)
	//for example, key="matrix", key="bubble"
	//for separating data, used separator variable, so records in a freame are: key,value,key,value,...
	void add_data_to_current_frame(string key, string value);
	void next_frame();
	string get_data_from_current_frame(string key);

	vector<string> file;

	string separator;	//separator for separating keys inside one frame (one frame is one string) 

	bool recording_ = false;
	bool playing_ = false;

	int frame_ = 0;
	string frame_data_;
};


