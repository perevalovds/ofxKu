#include "ofxKuMessageLog.h"

ofxKuMessageLog MLOGGER;

//--------------------------------------------------------------
void MLOG(const string &s) {	//add new message to log
	MLOGGER.log(s);
}

//--------------------------------------------------------------
void MLOG(const string &s, const ofColor &color) {
	MLOGGER.log(s, color);
}

//--------------------------------------------------------------
void MLOG(const string &s, const string &s_console) {
	MLOGGER.log(s, s_console);
}

//--------------------------------------------------------------
void MLOG(const string &s, const string &s_console, const ofColor &color) {
	MLOGGER.log(s, s_console, color);
}

//--------------------------------------------------------------
void ofxKuMessageLog::setup(string font_file) {
	if (disabled()) return;
	font.setup(font_file, 1.0, 1024, false, 8, 1.5); //1.0, 512, false); // , 8, 1.5);
	font.setCharacterSpacing(0);
}

//--------------------------------------------------------------
void ofxKuMessageLog::set_parameters(const ofxKuMessageLogParams &params) {
	if (disabled()) return; 
	this->params = params;
}

//--------------------------------------------------------------
void ofxKuMessageLog::clear() {
	if (disabled()) return; 
	lines.clear();
	colors.clear();
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s) {
	log(s, s, params.color);
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s, const ofColor &color) {
	log(s, s, color);
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s, const string &s_console) {	//different messages on the screen and console
	log(s, s_console, params.color);
}

//--------------------------------------------------------------
void ofxKuMessageLog::log(const string &s, const string &s_console, const ofColor &color) {
	cout << s_console << endl;

	if (disabled()) return; 
	lines.push_back(s);
	colors.push_back(color);

	remove_old_lines();
}

//--------------------------------------------------------------
void ofxKuMessageLog::remove_old_lines() {	//remove lines is lines.size exceeds capacity
	if (disabled()) return;
	while (lines.size() > params.capacity) {
		lines.erase(lines.begin());
		colors.erase(colors.begin());
	}
}

//--------------------------------------------------------------
void ofxKuMessageLog::big_message(ofxKuMessageLogBigMessage &big) {
	if (disabled()) return;
	big_ = big;
	big_.start();

}


//--------------------------------------------------------------
void ofxKuMessageLogBigMessage::start() {
	MLOG(message, message_console);

	time_end_ = ofGetElapsedTimef() + duration_sec;
}

//--------------------------------------------------------------
void ofxKuMessageLogBigMessage::draw(ofxFontStash &font) {
	float time = ofGetElapsedTimef();
	if (time <= time_end_) {
		ofFill();
		ofSetColor(color_back);
		ofRect(rect);
		ofFill();
		ofSetColor(color_font);
		font.setSize(font_size);
		ofRectangle r = font.getStringBoundingBox(message, 0, 0);
		ofPoint pos = rect.getCenter() - r.getCenter();
		font.drawMultiLine(message, font_size, pos.x, pos.y);
	}
}

//--------------------------------------------------------------
void ofxKuMessageLog::draw(float x, float y, float h, float font_size) {	//recomputes capacity using h
	if (disabled()) return;
	//update new capacity
	params.fit_capacity(h, font_size);
	remove_old_lines();

	ofFill();	//required for using ofxFontStash

	for (int i = 0; i < lines.size(); i++) {
		ofSetColor(colors[i]);
		font.draw(lines[i], font_size, x, y + i * font_size);
	}
	//string s = ofJoinString(lines, "\n");
	//font.drawMultiLine(s, params.size, params.pos.x, params.pos.y);

	//big message
	big_.draw(font);
}

//--------------------------------------------------------------
void ofxKuMessageLog::draw() { //uses params.capacity
	if (disabled()) return;

	//ofSetColor(params.color);
	//font.draw("Три", 30, x, y);
	ofFill();	//required for using ofxFontStash
	ofPoint &pos = params.pos;
	for (int i = 0; i < lines.size(); i++) {
		ofSetColor(colors[i]);
		font.draw(lines[i], params.font_size, pos.x, pos.y + i * params.font_size);
	}
	//string s = ofJoinString(lines, "\n");
	//font.drawMultiLine(s, params.size, params.pos.x, params.pos.y);

	//big message
	big_.draw(font);
	
}

//--------------------------------------------------------------
