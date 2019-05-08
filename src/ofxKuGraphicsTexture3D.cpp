#include "ofxKuGraphicsTexture3D.h"


//Posts on 3D textures, using for making the code:
//https://stackoverflow.com/questions/13459770/glsl-sampler3d-in-vertex-shader-texture-appears-blank
//https://en.sfml-dev.org/forums/index.php?topic=23871.0
//https://community.khronos.org/t/opengl-3-x-glenable-gl-texture-2d-gl-invalid-enum/61405

//--------------------------------------------------------------
ofxKuGraphicsTexture3D::ofxKuGraphicsTexture3D() {

}

//--------------------------------------------------------------
ofxKuGraphicsTexture3D::~ofxKuGraphicsTexture3D() {
	clear();
}


//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::gl_error_check(const string &pass) {
	GLenum res = glGetError();
	if (res != GL_NO_ERROR) {
		cout << "ofxKuGraphicsTexture3D, OpenGL error at " << pass << ": " << res << endl;
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofxKuGraphicsTexture3D::warning(const string &message) {
	cout << "ofxKuGraphicsTexture3D warning: " << message << endl;
}

//--------------------------------------------------------------
//https://www.khronos.org/opengl/wiki/OpenGL_Error

void GLAPIENTRY
Texture3D_OpenGL_MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

//--------------------------------------------------------------
void ofxKuGraphicsTexture3D::gl_errors_print_all() {
	cout << "Activating callback for OpenGL errors print" << endl;
	glEnable(GL_DEBUG_OUTPUT);
	gl_error_check("GL_DEBUG_OUTPUT");
	glDebugMessageCallback(Texture3D_OpenGL_MessageCallback, 0);
	gl_error_check("glDebugMessageCallback");
}


//--------------------------------------------------------------
void ofxKuGraphicsTexture3D::set_wrap_mode(int mode) { //WRAP_CLAMP, WRAP_REPEAT, WRAP_MIRROR_REPEAT
	wrap_mode_ = mode;
}

//--------------------------------------------------------------
void ofxKuGraphicsTexture3D::set_interpolate_mode(int mode) {
	interpolate_mode_ = mode;
}

//--------------------------------------------------------------
GLint ofxKuGraphicsTexture3D::gl_wrap(int mode) {	//mode - WRAP_CLAMP,...
	//Clamp modes
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexParameter.xml
	//GL_CLAMP,	GL_REPEAT, GL_MIRRORED_REPEAT (GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE)

	if (mode == WRAP_CLAMP) return GL_CLAMP;
	if (mode == WRAP_REPEAT) return GL_REPEAT;
	if (mode == WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;

	warning("Unknown mode in gl_wrap: " + ofToString(mode));

	return GL_CLAMP;
}

//--------------------------------------------------------------
GLint ofxKuGraphicsTexture3D::gl_interpolate(int mode) { //mode - INTERPOLATE_MODE_LINEAR,...
	//Interpolation
	//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
	//GL_LINEAR,GL_NEAREST
	if (mode == INTERPOLATE_MODE_LINEAR) return GL_LINEAR;
	if (mode == INTERPOLATE_MODE_NEAREST) return GL_NEAREST;

	warning("Unknown mode in gl_interpolate: " + ofToString(mode));

	return GL_LINEAR;
}

//--------------------------------------------------------------
GLint ofxKuGraphicsTexture3D::gl_type(int type) {
	if (type == TYPE_U8) return GL_UNSIGNED_BYTE;
	if (type == TYPE_U16) return GL_UNSIGNED_BYTE;
	if (type == TYPE_FLOAT) return GL_UNSIGNED_BYTE;
	warning("Unknown type in gl_type: " + ofToString(type));
	return GL_UNSIGNED_BYTE;
}

//--------------------------------------------------------------
unsigned int ofxKuGraphicsTexture3D::gl_type_size_in_bytes(int type) {
	if (type == TYPE_U8) return 1;
	if (type == TYPE_U16) return 2;
	if (type == TYPE_FLOAT) return 4;
	warning("Unknown type in gl_type_size_in_bytes: " + ofToString(type));
	return 1;
}

//--------------------------------------------------------------
GLint ofxKuGraphicsTexture3D::gl_channels(int channels) {	//1,3,4
	if (channels == 1) return GL_LUMINANCE;
	if (channels == 3) return GL_RGB;
	if (channels == 4) return GL_RGBA;
	warning("Bad channels in gl_channels: " + ofToString(channels));
	return GL_RGB;
}



//--------------------------------------------------------------
#define TEXTURE3D_ERR(s) {if (!gl_error_check(s)) return false;}

//Create from an existing array	
	//channels = 1,3,4
	//w,h,d - must be not less than 256 on some GPUs
bool ofxKuGraphicsTexture3D::load_data(int type, void* data, int w, int h, int d, int channels) {
	clear();

	glGenTextures(1, &textureID);
	TEXTURE3D_ERR("glGenTextures(1, &textureID)");

	glBindTexture(GL_TEXTURE_3D, textureID);
	TEXTURE3D_ERR("glBindTexture(GL_TEXTURE_3D, textureID);");

	if (!(glIsTexture(textureID) == GL_TRUE)) cout << "Texture Binding Failed." << endl;
	TEXTURE3D_ERR("glIsTexture");


	//Wrap modes
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexParameter.xml
	//GL_CLAMP,	GL_REPEAT, GL_MIRRORED_REPEAT (GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE)
	GLint wrap = gl_wrap(wrap_mode_);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
	TEXTURE3D_ERR("glTexParameteri GL_TEXTURE_WRAP");


	//Interpolation
	//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
	//GL_LINEAR,GL_NEAREST
	GLint interp = gl_interpolate(interpolate_mode_);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, interp);
	TEXTURE3D_ERR("glTexParameteri GL_TEXTURE_MAG_FILTER");
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, interp);
	TEXTURE3D_ERR("glTexParameteri GL_TEXTURE_MIN_FILTER");

	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	
	w_ = w;
	h_ = h;
	d_ = d;

	glTexImage3D(GL_TEXTURE_3D, 0, gl_channels(channels), w_, h_, d_, 0, gl_channels(channels), gl_type(type), data);
	TEXTURE3D_ERR("glTexImage3D");

	glBindTexture(GL_TEXTURE_3D, 0);
	TEXTURE3D_ERR("glBindTexture back to 0");

	return true;
}

//--------------------------------------------------------------

bool ofxKuGraphicsTexture3D::load_data(unsigned char *data, int w, int h, int d, int channels) {
	return load_data(TYPE_U8, data, w, h, d, channels);
}

//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::load_data(unsigned short *data, int w, int h, int d, int channels) {
	return load_data(TYPE_U16, data, w, h, d, channels);
}

//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::load_data(float *data, int w, int h, int d, int channels) {
	return load_data(TYPE_FLOAT, data, w, h, d, channels);
}


//--------------------------------------------------------------
//Clear
void ofxKuGraphicsTexture3D::clear() {
	if (loaded_) {
		glDeleteTextures(1, (GLuint *)&textureID);
		loaded_ = 0;
	}
}

//--------------------------------------------------------------
//Load file
//File Format:
//int w,h,d
//int type: 0 - u8, 10 - u16, 20 - float
//int channels = 1,3,4
//void* data
bool ofxKuGraphicsTexture3D::load_file(string file_name) {
	FILE *f = fopen(ofToDataPath(file_name).c_str(), "rb");
	if (f) {
		int w = 0;
		int h = 0;
		int d = 0;
		int type = 0;
		int channels = 0;
		fread(&w, sizeof(w), 1, f);
		fread(&h, sizeof(h), 1, f);
		fread(&d, sizeof(d), 1, f);
		fread(&type, sizeof(type), 1, f);
		fread(&channels, sizeof(channels), 1, f);
		int max_size = 16768;
		if (!(w >= 0 && w < max_size && h >= 0 && h < max_size && d >= 0 && d < max_size)) {
			warning("Loading file " + file_name + ": bad dimensions "
				+ ofToString(w) + " " + ofToString(h) + ofToString(d));
			fclose(f);
			return false;
		}
		if (type != TYPE_U8 && type != TYPE_U16 && type != TYPE_FLOAT) {
			warning("Loading file " + file_name + ": bad type " + ofToString(type));
			fclose(f);
			return false;
		}
		if (channels != 1 && channels != 3 && channels != 4) {
			warning("Loading file " + file_name + ": bad channels " + ofToString(channels));
			fclose(f); 
			return false;
		}

		unsigned int size = unsigned int(w) * unsigned int(h) * unsigned int(d)
			* unsigned int(gl_type_size_in_bytes(type)) * unsigned int(channels);
		unsigned char *data = new unsigned char[size];
		if (!data) {
			warning("Loading file " + file_name + ": not enough CPU memory");
			fclose(f);
			return false;
		}
		unsigned int read = fread(data, 1, size, f);
		if (read != size) {
			warning("Loading file " + file_name + ": not read the whole file");
			delete[] data;
			fclose(f);
			return false;
		}

		bool res = load_data(type, data, w, h, d, channels);
		delete[] data;
		fclose(f);
		return res;
	}
	else {
		warning("Can't load " + file_name);
		return false;
	}

}

//--------------------------------------------------------------
//Create file - static functions
bool ofxKuGraphicsTexture3D::save_file(int type, void* data, int w, int h, int d, int channels, string file_name) {
	FILE *f = fopen(ofToDataPath(file_name).c_str(), "wb");
	if (f) {
		fwrite(&w, sizeof(w), 1, f);
		fwrite(&h, sizeof(h), 1, f);
		fwrite(&d, sizeof(d), 1, f);
		fwrite(&type, sizeof(type), 1, f);
		fwrite(&channels, sizeof(channels), 1, f);

		unsigned int size = unsigned int(w) * unsigned int(h) * unsigned int(d)
			* unsigned int(gl_type_size_in_bytes(type)) * unsigned int(channels);
		unsigned int written = fwrite(data, 1, size, f);
		if (written != size) {
			warning("Save file " + file_name + ", can't write the whole file");
		}

		fclose(f);
		return true;
	}
	else {
		warning("Can't save " + file_name);
		return false;
	}
}

//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::save_file(unsigned char *data, int w, int h, int d, int channels, string file_name) {
	return save_file(TYPE_U8, data, w, h, d, channels, file_name);
}


//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::save_file(unsigned short *data, int w, int h, int d, int channels, string file_name) {
	return save_file(TYPE_U16, data, w, h, d, channels, file_name);
}

//--------------------------------------------------------------
bool ofxKuGraphicsTexture3D::save_file(float *data, int w, int h, int d, int channels, string file_name) {
	return save_file(TYPE_FLOAT, data, w, h, d, channels, file_name);

}

//--------------------------------------------------------------
void ofxKuGraphicsTexture3D::setUniformTexture(ofShader &shader, string name, int textureLocation) {	//textureLocation = 1,2,3,... - for all textures must be different
	//glEnable(GL_TEXTURE_3D);	//commented, because is not required for programmable pipeline

	glActiveTexture(GL_TEXTURE0 + textureLocation);
	gl_error_check("setUniformTexture, glActiveTexture");
	glBindTexture(GL_TEXTURE_3D, textureID);
	gl_error_check("setUniformTexture, GL_TEXTURE_3D");

	shader.setUniform1i("densityVol", textureLocation);
	gl_error_check("setUniformTexture, setUniform1i");

	gl_error_check("setUniformTexture, glActiveTexture GL_TEXTURE0");
	glActiveTexture(GL_TEXTURE0);
	
}

//--------------------------------------------------------------
