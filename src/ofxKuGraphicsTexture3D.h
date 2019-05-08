#pragma once

#include "ofMain.h"

//Class for working with 3D texture in GPU and using it in a shader,
//Intended for modern OpenGL/Programmable pipeline (OpenGL >= 3.2)

//Inspired by ofxVolumetrics addon by Timothy Scaffidi
//https://github.com/timscaffidi/ofxVolumetrics/blob/master/src/ofxTexture3d.h



struct ofxKuGraphicsTexture3D {
	ofxKuGraphicsTexture3D();
    ~ofxKuGraphicsTexture3D();

	//Set verbatim mode - enable if something not works you need to see GL messages
	void set_print_gl_errors(bool v) { print_errors_ = v; }

	static void set_print_gl_errors_all();	//Print all gl errors using callback

	//Set wrap mode - must be called before loading data into texture
	static const int WRAP_CLAMP = 0;		//default value
	static const int WRAP_REPEAT = 1;
	static const int WRAP_MIRRORED_REPEAT = 2;

	void set_wrap_mode(int mode); //WRAP_CLAMP, WRAP_REPEAT, WRAP_MIRROR_REPEAT

	//Set interpolate mode - must be called before loading data into texture
	static const int INTERPOLATE_MODE_LINEAR = 10;  //default value
	static const int INTERPOLATE_MODE_NEAREST = 11;
	void set_interpolate_mode(int mode);

	
	//Create from an existing array	
	//channels = 1,3,4
	//w,h,d - must be not less than 256 on some GPUs
	//array data is uploaded to GPU, so you can destroy it after loading
	bool load_data(unsigned char *data, int w, int h, int d, int channels);
	bool load_data(unsigned short *data, int w, int h, int d, int channels);
	bool load_data(float *data, int w, int h, int d, int channels);

	//Clear
	void clear();

	//Load file
	bool load_file(string file_name);

	//File Format:
	//int w,h,d
	//int type: 0 - u8, 1 - u16, 2 - float
	//int channels = 1,3,4
	//void* data

	//Create file - static functions
	static bool save_file(unsigned char *data, int w, int h, int d, int channels, string file_name);
	static bool save_file(unsigned short *data, int w, int h, int d, int channels, string file_name);
	static bool save_file(float *data, int w, int h, int d, int channels, string file_name);

	//Use in shader, for Programmable render
	void setUniformTexture(ofShader &shader, string name, int textureLocation);	//location = 1,2,3,... - for all textures must be different
	
	//Is texture loaded
	bool isLoaded() { return loaded_; }
	
	//Dimensions
	int w() { return w_; }
	int h() { return h_; }
	int d() { return d_; }

	//Static GL-related functions -----------------------------------------------
	//GL values
	static GLint gl_wrap(int mode);	//mode - WRAP_CLAMP,...
	static GLint gl_interpolate(int mode); //mode - INTERPOLATE_MODE_LINEAR,...
	static GLint gl_type(int type);
	static unsigned int gl_type_size_in_bytes(int type);

	static const int TYPE_U8 = 0;
	static const int TYPE_U16 = 10;
	static const int TYPE_FLOAT = 20;

	//GL cnahhels
	static GLint gl_channels(int channels);	//1,3,4

	//------------------------------------------------------------------------------

protected:
	int loaded_ = 0;

	GLuint textureID = 0;

	int w_ = 0;
	int h_ = 0;
	int d_ = 0;

	bool print_errors_ = false;
	bool gl_error_check(const string &pass);	//print OpenGL errors after operations
	static void warning(const string &message);


	//wrap and interpolation modes
	int wrap_mode_ = WRAP_CLAMP;	
	int interpolate_mode_ = INTERPOLATE_MODE_LINEAR;	

	bool load_data(int type, void* data, int w, int h, int d, int channels);

	static bool save_file(int type, void* data, int w, int h, int d, int channels, string file_name);
};