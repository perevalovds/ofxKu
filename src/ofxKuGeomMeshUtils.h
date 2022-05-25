#pragma once

//ofxKuGeomMeshUtils - utilities for mesh
//- loading OBJ files
//- computing normals
//- make wireframed mesh from triangle mesh
//- shuffle verties and triangles

#include "ofMain.h"

void ofxKuLoadObjFile( ofMesh &mesh, string fileName, bool useTex,
                   bool setupNormals, bool normalize,
                   bool separateFaces, bool shuffle, int texW, int texH//,
//				   bool caching_to_ply = true
);


void ofxKuSaveObjFile(ofMesh &mesh, string fileName, bool setupNormals, 
	bool textured, string mtl_file ="", int texW=0, int texH=0
);		//sets normals and so change mesh!


void ofxKuCreateWireframe(ofMesh &mesh, ofMesh &mesh_out);	//for triangle mesh

// Remove duplicated vertices - it occurs for IcoPrimitive, for example
void ofxKuMeshRemoveDuplicates(ofMesh& mesh_in, ofMesh& mesh_out, float eps = 0.001, bool verbose_duplicated = false);

// Set normals
void ofxKuSetNormals(ofMesh &mesh, bool invert = false, bool remove_duplicates = false, float eps = 0.001, bool verbose_duplicated = false);

// Shuffle vertices and triangles
void ofxKuMeshShuffle( vector<glm::vec3> &v, vector<ofIndexType> &t, vector<glm::vec2> &tex,
                   bool useTex = true, float shuffle_count=2 );

//-------------------------







