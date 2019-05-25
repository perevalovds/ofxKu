#pragma once

#include "ofMain.h"

//Recording and reading data into text file

class Recorder {
public:
	Recorder() {
		delimiter = ";";
	}

	void setup(string folder);

	void record_toggle();
	void play_toggle();

	//adds data to frame
	//for example, key="matrix", key="bubble"
	//for separating data, used delimiter, so records in a freame are: key,value,key,value,...
	void add_data_to_current_frame(string key, string value);
	void next_frame();
	string get_data_from_current_frame(string key);

	vector<string> file;
	void save();
	void load();

	string delimiter;

	bool recording_ = false;
	bool playing_ = false;
	string folder_;
	int frame_ = 0;
	string frame_data_;
};


