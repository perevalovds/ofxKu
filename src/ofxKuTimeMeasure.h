#pragma once

//Time measurement

#include "ofMain.h"


struct ofxKuTimeMeasure {
	enum MeasureMode : int {
		Seconds = 0,
		MilliSeconds = 1
	};

	void start(MeasureMode mode = MeasureMode::Seconds);
	void startSeconds();
	void startMilliSeconds();
	float measure();	//can call multiple times
	void measure(float &out_duration); //can call multiple times
	void measure(string &out_duration); //can call multiple times

private:
	float time_ = 0;
	MeasureMode measureMode_ = MeasureMode::Seconds;
	inline float secondsToOutput(const float& sec);
};


