#include "ofxKuVideoMapping.h"

ofxKuVideoMapping MAPPING;

//--------------------------------------------------------------
//Create shader
bool ofxKuVideoMapping::setup() {
	loaded_ = shader_.load("shaders/mapping");
	return loaded_;
}

//--------------------------------------------------------------
bool ofxKuVideoMapping::draw_texture(ofTexture &texture, float w, float h, ofxKuVideoMappingSettings &settings) {
	
	if (!loaded_) return false;
	ofxKuVideoMappingSettings settings1 = settings;
	settings1.texw = texture.getWidth();
	settings1.texh = texture.getHeight();

	bool res;
	res = begin_shader(settings1);
	if (!res) return false;
	texture.draw(0, 0, w, h);
	res = end_shader();

	return res;
}

//--------------------------------------------------------------
//Start correcting shader
//w, h - rendered image size
//V00 V10 - normalized destination vertices (0,1) for corners of rectangle (0,0)-(w,h)
//V01 V11
bool ofxKuVideoMapping::begin_shader(ofxKuVideoMappingSettings &settings) {

	if (!loaded_) return false;
	if (!begin_shader_) {
		begin_shader_ = 1;

		shader_.begin();
		shader_.setUniform2f("image_res", glm::vec2(settings.texw, settings.texh));
		shader_.setUniform1f("alpha", settings.alpha);
		shader_.setUniform2f("V00", glm::vec2(settings.V00));
		shader_.setUniform2f("V10", glm::vec2(settings.V10));
		shader_.setUniform2f("V01", glm::vec2(settings.V01));
		shader_.setUniform2f("V11", glm::vec2(settings.V11));

		return true;
	}
	return false;
}

//--------------------------------------------------------------
//End
bool ofxKuVideoMapping::end_shader() {
	if (!loaded_) return false; 
	if (begin_shader_) {
		begin_shader_ = 0;
		shader_.end();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
//Renders grid with color and line width set by user before calling this
/* void ofxKuVideoMapping::draw_grid(float w, float h, int nx, int ny) {
	if (nx > 0) {
		for (int i = 0; i < nx; i++) {
			float x = ofMap(i, 0, nx - 1, 0, w - 1);
			ofLine(x, 0, x, h);
		}
	}
	if (ny > 0) {
		for (int i = 0; i < ny; i++) {
			float y = ofMap(i, 0, ny - 1, 0, h - 1);
			ofLine(0, y, w, y);
		}
	}
}
*/

//--------------------------------------------------------------
