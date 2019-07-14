#pragma once

//Moving, rotating and cropping point clouds

#include "ofMain.h"

/*
struct ofxKuPcManipulate {	
	void setup(string matrix = "");	//string describing matrix 4x4, a11 a12 ... a
	string packToString();			//for saving somethere

	void update(bool edit);	//если не edit, то scale все равно редактируется, так как он общий
	static void update_finish_all();	//reset all params gui, related to calibration - call it after all calibrs finished

	void draw(vector<ofPoint> &pnt, ofColor color);

	void reset_main();
	void apply();	//apply all modifications

	void transform(vector<ofPoint> &points_in, vector<ofPoint> &points_out);

	ofMatrix4x4 get_matrix();


protected:
	//sequential operations
	void rotate(ofPoint direction, float degrees);
	void move(float x, float y, float z);
	void change_scale(float scl);
	void change_scale(float sclx, float scly, float sclz);	//for flips


	ofMatrix4x4 matrix_main_;

};

*/
