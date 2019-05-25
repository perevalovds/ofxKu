#include "ofxKuRecorderStrings.h"
#include "ofxKuFile.h"

//--------------------------------------------------------------
void ofxKuRecorderStrings::setup(string folder) {
	folder_ = folder;
	cout << "Recorder initialized in folder " << folder_ << endl;
}

//--------------------------------------------------------------
void ofxKuRecorderStrings::record_toggle() {
	if (playing_) {
		cout << "Can't record, because playing is on" << endl;
		return;
	}
	recording_ = !recording_;
	cout << "recording " << recording_ << endl;
	if (recording_) {
		file.clear();
		frame_ = 0;
		frame_data_ = "";
	}
	else {
		string file_name1 = folder_ + "record.txt";
		string file_name2 = folder_ + "record_" + ofGetTimestampString() + ".txt";
		ofxKuFileWriteStrings(file, file_name1);
		ofxKuFileWriteStrings(file, file_name2);
		cout << "Saved " << file_name2 << endl;
	}
}

//--------------------------------------------------------------
void ofxKuRecorderStrings::play_toggle() {
	if (recording_) {
		cout << "Can't play, because recording is on" << endl;
		return;
	}

	playing_ = !playing_;
	cout << "playing " << playing_ << endl;
	if (playing_) {
		string file_name1 = folder_ + "record.txt";
		file = ofxKuFileReadStrings(file_name1);
		frame_ = 0;
		frame_data_ = "";
	}
	else {
	}

}

//--------------------------------------------------------------
void ofxKuRecorderStrings::add_data_to_current_frame(string key, string value) {
	if (!frame_data_.empty()) frame_data_ += delimiter;
	frame_data_ += key + delimiter + value;
}

//--------------------------------------------------------------
string ofxKuRecorderStrings::get_data_from_current_frame(string key) {
	vector<string> item = ofSplitString(frame_data_, delimiter);  //TODO can work slow for big data and big number of keys
	for (int i = 0; i < item.size(); i+=2) {
		if (item[i] == key && (i + 1 < item.size())) {
			return item[i+1];
		}
	}
	return "";
}

//--------------------------------------------------------------
void ofxKuRecorderStrings::next_frame() {
	if (recording_) {
		file.push_back(frame_data_);
		frame_++;
	}
	if (playing_) {
		frame_++;
		if (frame_ >= file.size()) {
			play_toggle();
		}
	}
}

//--------------------------------------------------------------
void ofxKuRecorderStrings::save() {

}

//--------------------------------------------------------------
void ofxKuRecorderStrings::load() {

}

//--------------------------------------------------------------
