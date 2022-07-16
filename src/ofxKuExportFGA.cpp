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
