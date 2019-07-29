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

void ofxKuSetNormals( ofMesh &mesh );

//shuffle vertices and triangles
void ofxKuMeshShuffle( vector<glm::vec3> &v, vector<ofIndexType> &t, vector<glm::vec2> &tex,
                   bool useTex = true, float shuffle_count=2 );

//-------------------------







