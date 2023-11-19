#pragma once

//Buffer recorder/player, writes each data frame in separate file
//can be used for saving, for example, depth camera point clouds

#include "ofMain.h"

struct ofxKuRecorderBinary {
	ofxKuRecorderBinary();
	
	void start_rec(string folder);	//folder will be created
	void stop_rec();
	bool is_recording();

	void store_buffer(vector<unsigned char> &buffer);
	int frame() { return frame_; }
	void resetFrame() { frame_ = 0; }

protected:
	string folder_;
	bool recording_;
	int frame_;


};

struct ofxKuRecorderBinaryPlayer {
	ofxKuRecorderBinaryPlayer();
	void update();
	
	void start_play(string folder);	//folder must exists
	void set_fps(float fps);
	void stop_play();
	bool is_playing();
	bool isFrameNew();
	vector<unsigned char> &getBuffer();	//please use buffer before next update() calling

	float fps() { return fps_; }

	bool get_next_frame();
	int frame() { return frame_; }
	void resetFrame() { frame_ = 0; }

protected:
	string folder_;
	bool playing_;
	int frame_;
	vector<string> files;

	float fps_;

	float time_;
	float time_frame_;

	vector<unsigned char> buffer_;
	bool isFrameNew_;
};
