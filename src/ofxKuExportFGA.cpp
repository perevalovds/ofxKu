#include "ofxKuExportFGA.h"
#include "ofxKuFile.h"


//--------------------------------------------------------------------------------
void ofxKuExportFGA(string file_name,
	const vector<glm::vec3> vector_field,
	int nx, int ny, int nz, 
	glm::vec3 lower_bound, glm::vec3 upper_bound,
	int float_precision)
{
	cout << "Saving FGA " << nx << " x " << ny << " x " << nz << " to " << file_name << endl;
	int n = nx * ny * nz;
	if (n != vector_field.size()) {
		cout << "Bad array size!" << endl;
		return;
	}
	vector<string> file(n + 3);
	int k = 0;
	file[k++] = ofToString(nx) + "," + ofToString(ny) + "," + ofToString(nz) + ",";
	file[k++] = ofToString(lower_bound.x) + "," + ofToString(lower_bound.y) + "," + ofToString(lower_bound.z) + ",";
	file[k++] = ofToString(upper_bound.x) + "," + ofToString(upper_bound.y) + "," + ofToString(upper_bound.z) + ",";
	for (int z = 0; z < nz; z++) {
		for (int y = 0; y < ny; y++) {
			for (int x = 0; x < nx; x++) {
				auto& v = vector_field[x + nx * (y + ny * z)];
				file[k++] = ofToString(v.x, float_precision) 
					+ "," + ofToString(v.y, float_precision) 
					+ "," + ofToString(v.z, float_precision)
					+ ",";
			}
		}
	}

	ofxKuFileWriteStrings(file, file_name);
	cout << "   ...done" << endl;
}

//--------------------------------------------------------------------------------
void ofxKuExportVectorFieldAsAtlasImage(string tif_file_name,
	const vector<glm::vec3> vector_field,
	int nx, int ny, int nz, float max_field_value)
{
	if (!(nx == ny && ny == nz && nz == 64)) {
		cout << "ofxKuExportVectorFieldAsImage error - only 64x64x64 vector images are supported" << endl;
		return;
	}
	int n = nx;
	int atlas_count = 8;
	int w = atlas_count * n;
	int h = w;
	ofFloatPixels pix;
	pix.allocate(w, h, 3);
	for (int z = 0; z < nz; z++) {
		int atlas_x = (z % atlas_count)*nx;
		int atlas_y = (z / atlas_count)*ny;
		for (int y = 0; y < ny; y++) {
			for (int x = 0; x < nx; x++) {
				auto& v = vector_field[x + nx * (y + ny * z)];
				float r = ofMap(v.x, -max_field_value, max_field_value, 0, 1, true);
				float g = ofMap(v.y, -max_field_value, max_field_value, 0, 1, true);
				float b = ofMap(v.z, -max_field_value, max_field_value, 0, 1, true);
				pix.setColor(x + atlas_x, y + atlas_y, ofFloatColor(r, g, b));
			}
		}
	}

	ofSaveImage(pix, tif_file_name);

}
