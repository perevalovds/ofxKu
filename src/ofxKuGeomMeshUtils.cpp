#include "ofxKuGeomMeshUtils.h"
#include "ofxKuFile.h"
//#include "ofxKuFileTime.h"


//--------------------------------------------------------
void ofxKuLoadObjFile(ofMesh& mesh, string fileName, bool useTex,
	int setupNormals, bool normalize,
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
				glm::vec3& p = v[i];
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
			glm::vec3& p = v[j];
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
	if (setupNormals) {
		bool invert = (setupNormals > 0) ? false : true;
		ofxKuSetNormals(mesh, invert);
	}

	//write
	//if (caching_to_ply)  mesh.save(fileNamePly);
	//}
	//else {
	//	cout << "  loading cached " << fileNamePly << endl;
	//	mesh.load(fileNamePly);
	//}
}

//--------------------------------------------------------
void ofxKuSaveObjFile(ofMesh& mesh, string fileName, int setupNormals,
	bool textured, string mtl_file, int texW, int texH) {	//sets normals and so change mesh!

	auto& v = mesh.getVertices();
	int n = v.size();

	auto& vt = mesh.getTexCoords();
	auto& vn = mesh.getNormals();

	vector<GLuint>& ind = mesh.getIndices();
	int m = ind.size() / 3;

	if (setupNormals) {
		bool invert = (setupNormals > 0) ? false : true;
		ofxKuSetNormals(mesh, invert);
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
		string a1 = ofToString(ind[i * 3] + 1);
		string b1 = ofToString(ind[i * 3 + 1] + 1);
		string c1 = ofToString(ind[i * 3 + 2] + 1);
		string a = a1 + "/" + ((textured) ? a1 : "") + ((setupNormals) ? "/" + a1 : "");
		string b = b1 + "/" + ((textured) ? b1 : "") + ((setupNormals) ? "/" + b1 : "");
		string c = c1 + "/" + ((textured) ? c1 : "") + ((setupNormals) ? "/" + c1 : "");
		list[j++] = "f " + a + " " + b + " " + c;
	}

	std::cout << "Writing " << fileName << "..." << endl;
	ofxKuFileWriteStrings(list, fileName);
	std::cout << "Ok saving " << fileName << endl;

}


//--------------------------------------------------------
//shuffle vertices and triangles
void ofxKuMeshShuffle(vector<glm::vec3>& v, vector<ofIndexType>& t, vector<glm::vec2>& tex,
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
void ofxKuMeshRemoveDuplicates(ofMesh& mesh, float eps, bool verbose_duplicated) {
	ofMesh temp = mesh;
	ofxKuMeshRemoveDuplicates(temp, mesh, eps, verbose_duplicated);
}

//--------------------------------------------------------
// Remove duplicated vertices - it occurs for IcoPrimitive, for example
void ofxKuMeshRemoveDuplicates(ofMesh& mesh_in, ofMesh& mesh_out, float eps, bool verbose_duplicated) {
	if (verbose_duplicated) {
		cout << "Removing duplicated vertices, input vertices " << mesh_in.getNumVertices() << endl;
	}
	// Build vertices
	const float eps2 = eps * eps;
	auto& input = mesh_in.getVertices();
	vector<int> remap(input.size());
	vector<int> indices;
	vector<glm::vec3> pnt;

	for (int i = 0; i < input.size(); i++) {
		bool unique = true;
		for (int j = 0; j < pnt.size(); j++) {
			if (glm::distance2(input[i], pnt[j]) < eps2) {
				unique = false;
				remap[i] = j;

				if (verbose_duplicated) {
					cout << "Duplicated vertex " << i << " -> " << j << endl;
				}
				break;
			}
		}
		if (unique) {
			pnt.push_back(input[i]);
			remap[i] = pnt.size() - 1;
			indices.push_back(i);
		}
	}

	// Build output
	if (pnt.size() == input.size()) {
		mesh_out = mesh_in;
		if (verbose_duplicated) {
			cout << "No duplicates" << endl;
		}
	}
	else {
		mesh_out.clear();
		// Vertices
		mesh_out.addVertices(pnt);
		// Normals
		if (mesh_in.getNumNormals() == pnt.size()) {
			for (int j = 0; j < pnt.size(); j++) {
				mesh_out.addNormal(mesh_in.getNormal(indices[j]));
			}
		}
		// Tex coords
		if (mesh_in.getNumTexCoords() == pnt.size()) {
			for (int j = 0; j < pnt.size(); j++) {
				mesh_out.addTexCoord(mesh_in.getTexCoord(indices[j]));
			}
		}
		// Triangles
		for (int i = 0; i < mesh_in.getNumIndices(); i++) {
			mesh_out.addIndex(remap[mesh_in.getIndex(i)]);
		}
	}
}
//--------------------------------------------------------
// Set normals
void ofxKuSetNormals(ofMesh& mesh, bool invert, bool remove_duplicates, float eps, bool verbose_duplicated) {
	if (remove_duplicates) {
		ofMesh mesh2 = mesh;
		ofxKuMeshRemoveDuplicates(mesh, mesh2, eps, verbose_duplicated);
		mesh = mesh2;
	}

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
		const ofPoint& v1 = mesh.getVertex(i1);
		const ofPoint& v2 = mesh.getVertex(i2);
		const ofPoint& v3 = mesh.getVertex(i3);

		//Compute the triangle's normal
		ofPoint dir = -((v2 - v1).crossed(v3 - v1)).normalized();

		//Accumulate it to norm array for i1, i2, i3
		norm[i1] += dir;
		norm[i2] += dir;
		norm[i3] += dir;
	}

	//Normalize the normal's length and invert if required
	for (int i = 0; i < nV; i++) {
		norm[i] = glm::normalize(norm[i]);
		if (invert) {
			norm[i] = -norm[i];
		}
	}

	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals(norm);
}

//--------------------------------------------------------
void ofxKuCreateWireframe(ofMesh& mesh, ofMesh& mesh_out) { //for triangle mesh

	auto& v = mesh.getVertices();
	int n = v.size();

	vector<GLuint>& ind = mesh.getIndices();
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
// Move and scale mesh vertices
void ofxKuMeshTransform(vector<glm::vec3>& v, glm::vec3 translate, glm::vec3 scale) {
	for (auto& vert : v) {
		vert = vert * scale + translate;
	}
}

//--------------------------------------------------------
glm::vec3 ofKuMeshSampleRandomPoint(const ofMesh& mesh)
{
	glm::vec3 result = glm::vec3(0, 0, 0);
	auto& V = mesh.getVertices();
	int n = V.size();
	auto& T = mesh.getIndices();
	int m = T.size() / 3;
	if (n == 0 || m == 0) {
		return result;
	}
	if (m * 3 != T.size()) {
		cout << "ERROR at ofKuMeshSampleRandomPoint - not triangle mesh" << endl;
		return result;
	}
	int t = 3 * int(ofRandom(m)); // Choose triangle
	float a, b, c;
	while (true) {
		// Choose weights
		a = ofRandom(1);
		b = ofRandom(1);
		c = ofRandom(1);
		float sum = a + b + c;
		if (sum > 0.0001) {
			a /= sum;
			b /= sum;
			c /= sum;
			break;
		}
	}
	result = V[T[t]] * a + V[T[t + 1]] * b + V[T[t + 2]] * c;
	return result;
}

//--------------------------------------------------------


