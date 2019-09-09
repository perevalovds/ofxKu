#pragma once

//Videomapping transform for quads

#include "ofMain.h"

struct ofxKuVideoMappingSettings {
	float texw = 0;			//in pixels
	float texh = 0;
	float alpha = 1;		//0..1
	ofVec2f V00;			//uniform, 0..1
	ofVec2f V10;
	ofVec2f V01;
	ofVec2f V11;
};

struct ofxKuVideoMapping {
	//Create shader
	bool setup();	
	
	//Note: color is controlled by alpha, not ofSetColor()
	bool draw_texture(ofTexture &texture, float w, float h, ofxKuVideoMappingSettings &settings);

	//Start mapping-correction shader
	//w, h - rendered image size
	//V00 V10 - destination vertices for corners of rectangle (0,0)-(w,h)
	//V01 V11
	//Note: color is controlled by alpha, not ofSetColor()
	bool begin_shader(ofxKuVideoMappingSettings &settings);

	//Renders grid with color and line width set by user before calling this
	//static void draw_grid(float w, float h, int nx, int ny);	

	//End
	bool end_shader();
protected:
	ofShader shader_;

	int loaded_ = 0;
	int begin_shader_ = 0;

};


extern ofxKuVideoMapping MAPPING;
