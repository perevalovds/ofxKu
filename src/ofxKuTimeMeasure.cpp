#include "ofxKuTimeMeasure.h"

//--------------------------------------------------------------
void ofxKuTimeMeasure::start(MeasureMode mode) {
	time_ = ofGetElapsedTimef();
	measureMode_ = mode;
}

//--------------------------------------------------------------
void ofxKuTimeMeasure::startSeconds() {
	start(MeasureMode::Seconds);
}

//--------------------------------------------------------------
void ofxKuTimeMeasure::startMilliSeconds() {
	start(MeasureMode::MilliSeconds);
}

//--------------------------------------------------------------
inline float ofxKuTimeMeasure::secondsToOutput(const float& sec)
{
	if (measureMode_ == MeasureMode::Seconds)
		return sec;
	else
		return sec * 1000.f;
}

//--------------------------------------------------------------
float ofxKuTimeMeasure::measure() {
	float time = ofGetElapsedTimef();
	float result = time - time_;
	time_ = time;
	return secondsToOutput(result);
}

//--------------------------------------------------------------
void ofxKuTimeMeasure::measure(float &out_duration) { //can call multiple times
	out_duration = measure();
}

//--------------------------------------------------------------
void ofxKuTimeMeasure::measure(string &out_duration) { //can call multiple times
	out_duration = ofToString(measure(), 4);
}

//--------------------------------------------------------------
