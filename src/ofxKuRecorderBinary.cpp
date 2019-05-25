#include "ofxKuRecorderBinary.h"

//--------------------------------------------------------------
ofxKuRecorderBinary::ofxKuRecorderBinary() {
	frame_ = 0;
	recording_ = false;
}

//--------------------------------------------------------------
void ofxKuRecorderBinary::start_rec(string folder) {	//folder must exists
	folder_ = folder;
	recording_ = true;
	frame_ = 0;
}

//--------------------------------------------------------------
void ofxKuRecorderBinary::stop_rec() {
	recording_ = false;
}

//--------------------------------------------------------------
bool ofxKuRecorderBinary::is_recording() {
	return recording_;
}

//--------------------------------------------------------------
void ofxKuRecorderBinary::store_buffer(vector<unsigned char> &buffer) {
	if (recording_) {
		string fileName = folder_ + "/" + ofToString(frame_,5,'0');
		fileName = ofToDataPath(fileName);
		//cout << "ofxKuBufferRecorder  store buffer " << buffer.size() << endl;
		FILE *file = fopen(fileName.c_str(),"wb");
		if (file) {
			fwrite(&buffer[0], 1, buffer.size(), file);
			frame_++;
			fclose(file);
		}
	}
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
ofxKuRecorderBinaryPlayer::ofxKuRecorderBinaryPlayer() {
	playing_ = false;
	fps_ = 30;

	time_ = 0;
	isFrameNew_ = false;
}

//--------------------------------------------------------------
void ofxKuRecorderBinaryPlayer::set_fps(float fps) {
	fps_ = fps;
}

//--------------------------------------------------------------
void ofxKuRecorderBinaryPlayer::start_play(string folder) {	//folder will be created
	folder_ = folder;
	playing_ = true;
	ofDirectory dir;
	int n = dir.listDir(folder);
	files.resize(n);
	for (int i=0; i<n; i++) {
		files[i] = dir.getPath(i);
	}
	
	time_frame_ = 0;
	frame_ = 0;

	//cout << "start play " << folder << " " << n << endl;
}

//--------------------------------------------------------------
void ofxKuRecorderBinaryPlayer::stop_play() {
	playing_ = false;
}

//--------------------------------------------------------------
bool ofxKuRecorderBinaryPlayer::is_playing() {
	return playing_;
}

//--------------------------------------------------------------
bool ofxKuRecorderBinaryPlayer::isFrameNew() {
	return is_playing() && isFrameNew_;
}

//--------------------------------------------------------------
vector<unsigned char> &ofxKuRecorderBinaryPlayer::getBuffer() {	//please use buffer before next update() calling
	return buffer_;
}

//--------------------------------------------------------------
bool ofxKuRecorderBinaryPlayer::get_next_frame() {
	int n = files.size();
	if (n==0) return false;
	int f = frame_ % n;
	frame_ = f;
	frame_++;
	string fileName = folder_ + "/" + ofToString(f,5,'0');
	fileName = ofToDataPath(fileName);
	FILE *file = fopen(fileName.c_str(),"rb");
	if (file) {
		ofFile file_f(fileName);
		size_t size = file_f.getSize();
		buffer_.resize(size);
		fread(&buffer_[0], 1, buffer_.size(), file);
		fclose(file);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void ofxKuRecorderBinaryPlayer::update() {
	if (is_playing()) {
		float time = ofGetElapsedTimef();
		float dt = ofClamp(time - time_, 0.001, 1);
		time_ = time;

		//cout << "ofxKuBufferPlayer::update(), dt " << dt << endl;
		isFrameNew_ = false;
		time_frame_ += dt;
		while (time_frame_ > 1.0 / fps_ && dt>0) {
			get_next_frame();
			time_frame_ -= 1.0 / fps_;
			isFrameNew_ = true;
		}
	}

}

//--------------------------------------------------------------
