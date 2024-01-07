#include "ofxKuPcManipulate.h"


//--------------------------------------------------------------
/*void ofxKuPcManipulate::setup(string matrix) {
	reset_main();
	vector<string> a = ofSplitString(matrix, " ");
	if (a.size() >= 16) {
		vector<float> v(16);
		for (int i = 0; i < 16; i++) {
			v[i] = ofToFloat(a[i]);
		}
		matrix_main_.set(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
	}
}

//--------------------------------------------------------------
void ofxKuPcManipulate::reset_main() {
	matrix_main_.makeIdentityMatrix();
	change_scale(0.001);
	change_scale(1, -1, 1);
}


//--------------------------------------------------------------
string ofxKuPcManipulate::packToString() {			//for saving somethere
	const float *v = matrix_main_.getPtr();
	string s = "";
	for (int i = 0; i < 16; i++) {
		if (i > 0) s += " ";
		s += ofToString(v[i]);
	}
	return s;
}

//--------------------------------------------------------------
void ofxKuPcManipulate::update(bool edit) {
	//scale редактируется всегда, а остальные - если edit

	change_scale(1 + PRMQD clb_mscl*0.001);

	if (edit) {
		if (PRMQD clb_RESET) {
			reset_main();
		}

		float mov = 1.0 / 2000.0;
		float rot = 1.0 / 10.0;
		move(PRMQD clb_mx * mov, PRMQD clb_my * mov, PRMQD clb_mz * mov);

		rotate(ofPoint(1, 0, 0), PRMQD clb_mrotx * rot);

		rotate(ofPoint(0, 1, 0), PRMQD clb_mroty * rot);

		rotate(ofPoint(0, 0, 1), PRMQD clb_mrotz * rot);

		if (PRMQD clb_flipX) {
			change_scale(-1, 1, 1);
		}
		if (PRMQD clb_flipY) {
			change_scale(1, -1, 1);
		}
		if (PRMQD clb_flipZ) {
			change_scale(1, 1, -1);
		}
	}

}

//--------------------------------------------------------------
void ofxKuPcManipulate::update_finish_all() {
	PRMQD clb_RESET = 0;
	PRMQD clb_mx = PRMQD clb_my = PRMQD clb_mz = 0;
	PRMQD	clb_mrotx = 0;
	PRMQD	clb_mroty = 0;
	PRMQD	clb_mrotz = 0;
	PRMQD clb_mscl = 0;
	PRMQD clb_flipX = 0;
	PRMQD clb_flipY = 0;
	PRMQD clb_flipZ = 0;
}


//--------------------------------------------------------------
void ofxKuPcManipulate::draw(vector<ofPoint> &pnt, ofColor color) {

	float W = ofGetWidth();
	float H = ofGetHeight();
	ofPushMatrix();
	ofTranslate(W / 2 + PRMQD clb_gui_x * W, H / 2 + PRMQD clb_gui_y * H);
	
	float scale = PRMQD clb_gui_scl * ofToFloat(PRMQD clb_gui_scl2);
	float scl = scale;// *H / 1080.0;
	ofScale(scl, scl, scl);

	int proj = PRMQD clb_proj; //XY, XZ, YZ, Custom
	if (proj == 1) {		//XY
		ofScale(1, 1, 0.001);	//TODO do real orthographic
	}
	if (proj == 2) {		//YZ
		ofRotateX(90);
		ofScale(1, 0.001, 1);	//TODO do real orthographic
	}
	if (proj == 3) {		//XZ
		ofRotateY(90);
		ofRotateX(90);
		ofScale(0.001, 1, 1);	//TODO do real orthographic		
	}
	if (proj == 4) {		//Custom
		ofRotateX(PRMQD clb_rotx);
		ofRotateY(PRMQD clb_roty);
		ofRotateZ(PRMQD clb_rotz);
	}

	ofMesh mesh;
	mesh.addVertices(pnt);
	//ofPushMatrix();
	//ofMultMatrix(get_matrix());
	ofSetColor(color);
	mesh.drawVertices();
	//ofPopMatrix();
	
	//guides
	ofSetColor(255, 0, 0);
	ofNoFill();
	ofSetCircleResolution(60);
	//ofCircle(0, 0, 1);
	float rx = PRMQD clb_w / 2;
	float ry = PRMQD clb_h / 2;
	ofLine(-rx, 0, 0, rx, 0, 0);
	ofLine(0, -ry, 0, 0, ry, 0);
	ofLine(0, 0, 0, 0, 0, PRMQD clb_Z);

	ofLine(-rx, 0, PRMQD clb_Z, rx, 0, PRMQD clb_Z);
	ofLine(0, -ry, PRMQD clb_Z, 0, ry, PRMQD clb_Z);

	ofRect(-PRMQD clb_w / 2, -PRMQD clb_h / 2, PRMQD clb_w, PRMQD clb_h);
	ofLine(-PRMQD clb_w / 2, 0, 0, -PRMQD clb_w / 2, 0, PRMQD clb_Z);
	ofLine(PRMQD clb_w / 2, 0, 0, PRMQD clb_w / 2, 0, PRMQD clb_Z);
	ofLine(0, -PRMQD clb_h / 2, 0, 0, -PRMQD clb_h / 2, PRMQD clb_Z);
	ofLine(0, PRMQD clb_h / 2, 0, 0, PRMQD clb_h / 2, PRMQD clb_Z);

	ofPopMatrix();

	
}

//--------------------------------------------------------------
ofMatrix4x4 ofxKuPcManipulate::get_matrix() {
	return matrix_main_;
}

//--------------------------------------------------------------
void ofxKuPcManipulate::transform(vector<ofPoint> &points_in, vector<ofPoint> &points_out) {
	int n = points_in.size();
	points_out = points_in;
	ofMatrix4x4 matrix = get_matrix();
	for (int i = 0; i < n; i++) {
		ofPoint &p = points_out[i];
		ofVec4f v = ofVec4f(p.x, p.y, p.z, 1.0) * matrix;
		//if (v.w != 0) {				//TODO отключена проверка деления на ноль
		p = ofPoint(v.x / v.w, v.y / v.w, v.z / v.w);
		//}
	}

	//удаление точек вне слоя
	if (PRMQD clipping) {
		//cout << n << " ... ";
		float radx = PRMQD clb_w / 2;
		float rady = PRMQD clb_h / 2;
		for (int i = 0; i < n; ) {
			ofPoint &p = points_out[i];
			if (p.z < 0 || p.z > PRMQD clb_Z || p.x < -radx || p.x > radx || p.y < -rady || p.y > rady) {
				swap(points_out[i], points_out[n - 1]);
				n--;
			}
			else i++;
		}
		points_out.resize(n);
		//cout << n << endl;
	}
}

//--------------------------------------------------------------
void ofxKuPcManipulate::rotate(ofPoint direction, float degrees) {
	if (degrees == 0) return;
	ofMatrix4x4 m;
	m.makeRotationMatrix(degrees, direction.x, direction.y, direction.z);
	matrix_main_ *= m;
}

//--------------------------------------------------------------
void ofxKuPcManipulate::move(float x, float y, float z) {
	if (x == 0 && y == 0 && z == 0) return;
	ofMatrix4x4 m;
	m.makeTranslationMatrix(ofPoint(x,y,z));
	matrix_main_ *= m;

}

//--------------------------------------------------------------
void ofxKuPcManipulate::change_scale(float scl) {
	if (scl == 1) return;
	change_scale(scl, scl, scl);
}

//--------------------------------------------------------------
void ofxKuPcManipulate::change_scale(float sclx, float scly, float sclz) {	//for flips
	ofMatrix4x4 m;
	m.makeScaleMatrix(sclx, scly, sclz);
	matrix_main_ *= m;
}

//--------------------------------------------------------------
 */
