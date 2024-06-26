#pragma once

//Export FGA file format for vector fields in Unreal Engine.

#include "ofMain.h"


// Export vector field vector_field of size nx * ny * nz, into FGA file file_name,
// using boundaries [lower_bound, upper_bound]
// float_ - how much digits use for output number to FGA text file.
void ofxKuExportFGA(string file_name, 
	const vector<glm::vec3> vector_field,
	int nx, int ny, int nz, 
	glm::vec3 lower_bound = glm::vec3(-1,-1,-1), glm::vec3 upper_bound = glm::vec3(-1, -1, -1),
	int float_precision = 5);

// Export vector field as image atlas: float-valued tif RGB [0..1].
// [-max_field_value,max_field_value] is mapped to [0..1]
// Please use tif file format in name, for example, "atlas.tif"
void ofxKuExportVectorFieldAsAtlasImage(string tif_file_name,
	const vector<glm::vec3> vector_field,
	int nx, int ny, int nz, float max_field_value = 1);


