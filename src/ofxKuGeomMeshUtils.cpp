#include "ofxKuGeomMeshUtils.h"
#include "ofxKuFile.h"
//#include "ofxKuFileTime.h"


//--------------------------------------------------------
void ofxKuLoadObjFile(ofMesh &mesh, string fileName, bool useTex,
	bool setupNormals, bool normalize,
	bool separateFaces, bool shuffle, int texW, int texH//,
   // bool caching_to_ply 
)
{
	cout << "loading " << fileName << "...";
	string fileNamePly = fileName + ".ply";
	//if (!caching_to_ply || ofxKuFileTime::isNewerThan(fileName, fileNamePly)) {
		//if (caching_to_ply) cout << "   rebuild cached version..." << endl;
	mesh.clear();
	vector<string> lines = ofxKuFileReadStrings(ofToDataPath(fileName));

	vector<glm::vec3> v;      //vertices
	vector<ofIndexType> t;  //indices for triangles
	vector<glm::vec2> tex;    //texture coords

	ofPoint p;
	int f[4];

	cout << "   parsing..." << endl;
	for (int i = 0; i < lines.size(); i++) {
		vector<string> list = ofSplitString(lines[i], " ", true, true);
		int n = list.size();
		if (n > 0) {
			if (list[0] == "v" && n >= 4) {
				v.push_back(glm::vec3(
					ofToFloat(list[1]),
					ofToFloat(list[2]),
					ofToFloat(list[3])
				));
			}
			if (useTex && list[0] == "vt" && n >= 3) {
				tex.push_back(glm::vec2(
					ofToFloat(list[1]) * texW,
					ofToFloat(list[2]) * texH
				));
			}
			if (list[0] == "f" && n >= 4) {
				int N = min(n - 1, 4);
				for (int j = 0; j < N; j++) {
					vector<string> line = ofSplitString(list[j + 1], "/", true, true);
					if (line.size() > 0) {
						f[j] = ofToInt(line[0]) - 1;

					}
				}
				t.push_back(f[0]);
				t.push_back(f[1]);
				t.push_back(f[2]);
				//4-угольная грань
				if (n >= 5) {
					t.push_back(f[0]);
					t.push_back(f[2]);
					t.push_back(f[3]);
				}
			}

		}
	}

	cout << "   processing..." << endl;
	//нормализация в куб [-1,1]x[-1,1]x[-1,1]
	if (normalize) {
		glm::vec3 p0 = ofPoint(0);
		glm::vec3 p1 = p0;
		if (v.size() > 0) {
			p0 = v[0];
			p1 = p0;
			for (int i = 0; i < v.size(); i++) {
				glm::vec3 &p = v[i];
				p0.x = min(p0.x, p.x);
				p0.y = min(p0.y, p.y);
				p0.z = min(p0.z, p.z);
				p1.x = max(p1.x, p.x);
				p1.y = max(p1.y, p.y);
				p1.z = max(p1.z, p.z);
			}
		}
		glm::vec3 c = (p0 + p1) * 0.5;
		glm::vec3 delta = p1 - p0;
		float scl = delta.x;
		scl = max(scl, delta.y);
		scl = max(scl, delta.z);
		if (scl > 0) {
			scl = 1.0 / scl;
		}
		for (int j = 0; j < v.size(); j++) {
			glm::vec3 &p = v[j];
			p = (p - c) * scl;
		}
	}

	//shuffle
	if (shuffle) {
		ofxKuMeshShuffle(v, t, tex, useTex);
	}

	//vertices
	mesh.addVertices(v);

	//texture coords
	if (useTex) {
		if (tex.size() >= v.size()) {
			mesh.addTexCoords(tex);
		}
		else {
			cout << "Error in OBJ model, not enough texture coords" << endl;
		}
	}

	//triangles
	mesh.addIndices(t);

	//normals
	if (setupNormals) { ofxKuSetNormals(mesh); }

	//write
	//if (caching_to_ply)  mesh.save(fileNamePly);
	//}
	//else {
	//	cout << "  loading cached " << fileNamePly << endl;
	//	mesh.load(fileNamePly);
	//}
}

//--------------------------------------------------------
void ofxKuSaveObjFile(ofMesh &mesh, string fileName, bool setupNormals,
	bool textured, string mtl_file, int texW, int texH) {	//sets normals and so change mesh!

	auto &v = mesh.getVertices();
	int n = v.size();

	auto &vt = mesh.getTexCoords();
	auto &vn = mesh.getNormals();

	vector<GLuint> &ind = mesh.getIndices();
	int m = ind.size() / 3;

	if (setupNormals) {
		ofxKuSetNormals(mesh);
	}
	int N = n + m;
	if (setupNormals) N += n;
	if (textured) N += n;

	vector<string> list(2 + N);	//header, v, vt, vn, f
	int j = 0;

	if (textured && !mtl_file.empty()) {
		list[j++] = "mtllib " + mtl_file; // +".mtl";
		list[j++] = "usemtl texture";
	}

	std::cout << "    v  " << n << "..." << endl;
	for (int i = 0; i < n; i++) {
		list[j++] = "v " + ofToString(v[i].x) + " " + ofToString(v[i].y) + " " + ofToString(v[i].z);
	}

	if (setupNormals) {
		std::cout << "    vn..." << endl;
		for (int i = 0; i < n; i++) {
			list[j++] = "vn " + ofToString(vn[i].x) + " " + ofToString(vn[i].y) + " " + ofToString(vn[i].z);
		}
	}

	if (textured) {
		std::cout << "    vt..." << endl;
		for (int i = 0; i < n; i++) {
			list[j++] = "vt " + ofToString(vt[i].x / texW) + " " + ofToString(vt[i].y / texH);
		}
	}


	std::cout << "    f..." << endl;
	for (int i = 0; i < m; i++) {
		string a = ofToString(ind[i * 3] + 1);
		string b = ofToString(ind[i * 3 + 1] + 1);
		string c = ofToString(ind[i * 3 + 2] + 1);
		a = a + "/" + a + "/" + a;
		b = b + "/" + b + "/" + b;
		c = c + "/" + c + "/" + c;
		list[j++] = "f " + a + " " + b + " " + c;
	}

	std::cout << "Writing " << fileName << "..." << endl;
	ofxKuFileWriteStrings(list, fileName);
	std::cout << "Ok saving " << fileName << endl;

}


//--------------------------------------------------------
//shuffle vertices and triangles
void ofxKuMeshShuffle(vector<glm::vec3> &v, vector<ofIndexType> &t, vector<glm::vec2> &tex,
	bool useTex, float shuffle_count) {

	int n = v.size();
	vector<int> vto(n);
	vector<int> vfrom(n);
	for (int i = 0; i < n; i++) {
		vto[i] = i;
		vfrom[i] = i;
	}
	//vertices
	int cnt = n * shuffle_count;
	for (int k = 0; k < cnt; k++) {
		int i = ofRandom(0, n);
		i = min(i, n - 1);
		int j = ofRandom(0, n);
		j = min(j, n - 1);
		if (i != j) {
			swap(vfrom[vto[i]], vfrom[vto[j]]);
			swap(vto[i], vto[j]);
		}
	}
	auto v1 = v;
	for (int i = 0; i < n; i++) {
		v[i] = v1[vto[i]];
	}
	if (useTex && tex.size() >= n) {
		auto tex1 = tex;
		for (int i = 0; i < n; i++) {
			tex[i] = tex1[vto[i]];
		}
	}
	//tune triangles
	vector<ofIndexType> t1 = t;
	for (int i = 0; i < t.size(); i++) {
		t[i] = vfrom[t1[i]];
	}

	//shuffle triangles
	int T = t.size() / 3;
	int count = T * 2;
	for (int k = 0; k < count; k++) {
		int i = ofRandom(0, T);
		i = min(i, T - 1);
		int j = ofRandom(0, T);
		j = min(j, T - 1);
		if (i != j) {
			int a = i * 3;
			int b = j * 3;
			swap(t[a], t[b]);
			swap(t[a + 1], t[b + 1]);
			swap(t[a + 2], t[b + 2]);
		}
	}
}


//--------------------------------------------------------
//установить нормали
void ofxKuSetNormals(ofMesh &mesh) {
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	vector<glm::vec3> norm(nV); //Array for the normals

	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
	for (int t = 0; t < nT; t++) {

		//Get indices of the triangle t
		int i1 = mesh.getIndex(3 * t);
		int i2 = mesh.getIndex(3 * t + 1);
		int i3 = mesh.getIndex(3 * t + 2);

		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex(i1);
		const ofPoint &v2 = mesh.getVertex(i2);
		const ofPoint &v3 = mesh.getVertex(i3);

		//Compute the triangle's normal
		ofPoint dir = ((v2 - v1).crossed(v3 - v1)).normalized();

		//Accumulate it to norm array for i1, i2, i3
		norm[i1] += dir;
		norm[i2] += dir;
		norm[i3] += dir;
	}

	//Normalize the normal's length
	for (int i = 0; i < nV; i++) {
		//norm[i].normalize();
		glm::normalize(norm[i]);
	}

	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals(norm);
}

//--------------------------------------------------------
void ofxKuCreateWireframe(ofMesh &mesh, ofMesh &mesh_out) { //for triangle mesh

	auto &v = mesh.getVertices();
	int n = v.size();

	vector<GLuint> &ind = mesh.getIndices();
	int m = ind.size() / 3;

	mesh_out = mesh;
	mesh_out.clearIndices();

	for (int i = 0; i < m; i++) {
		int i1 = ind[i * 3];
		int i2 = ind[i * 3 + 1];
		int i3 = ind[i * 3 + 2];
		mesh_out.addIndex(i1);
		mesh_out.addIndex(i2);
		mesh_out.addIndex(i2);

		mesh_out.addIndex(i2);
		mesh_out.addIndex(i3);
		mesh_out.addIndex(i3);

		mesh_out.addIndex(i3);
		mesh_out.addIndex(i1);
		mesh_out.addIndex(i1);
	}

}

//--------------------------------------------------------


