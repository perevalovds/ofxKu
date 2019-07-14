#include "Calibrate3D.h"
#include "gui_generated.h"

//--------------------------------------------------------------
void Calibrate3D::setup(string matrix) {
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
void Calibrate3D::reset_main() {
	matrix_main_.makeIdentityMatrix();
	change_scale(0.001);
	change_scale(1, -1, 1);
}


//--------------------------------------------------------------
string Calibrate3D::packToString() {			//for saving somethere
	const float *v = matrix_main_.getPtr();
	string s = "";
	for (int i = 0; i < 16; i++) {
		if (i > 0) s += " ";
		s += ofToString(v[i]);
	}
	return s;
}

//--------------------------------------------------------------
void Calibrate3D::update(bool edit) {
	//scale редактируется всегда, а остальные - если edit

	change_scale(1 + PRM clb_mscl*0.001);

	if (edit) {
		if (PRM clb_RESET) {
			reset_main();
		}

		float mov = 1.0 / 2000.0;
		float rot = 1.0 / 10.0;
		move(PRM clb_mx * mov, PRM clb_my * mov, PRM clb_mz * mov);

		rotate(ofPoint(1, 0, 0), PRM clb_mrotx * rot);

		rotate(ofPoint(0, 1, 0), PRM clb_mroty * rot);

		rotate(ofPoint(0, 0, 1), PRM clb_mrotz * rot);

		if (PRM clb_flipX) {
			change_scale(-1, 1, 1);
		}
		if (PRM clb_flipY) {
			change_scale(1, -1, 1);
		}
		if (PRM clb_flipZ) {
			change_scale(1, 1, -1);
		}
	}

}

//--------------------------------------------------------------
void Calibrate3D::update_finish_all() {
	PRM clb_RESET = 0;
	PRM clb_mx = PRM clb_my = PRM clb_mz = 0;
	PRM	clb_mrotx = 0;
	PRM	clb_mroty = 0;
	PRM	clb_mrotz = 0;
	PRM clb_mscl = 0;
	PRM clb_flipX = 0;
	PRM clb_flipY = 0;
	PRM clb_flipZ = 0;
}


//--------------------------------------------------------------
void Calibrate3D::draw(vector<ofPoint> &pnt, ofColor color) {

	float W = ofGetWidth();
	float H = ofGetHeight();
	ofPushMatrix();
	ofTranslate(W / 2 + PRM clb_gui_x * W, H / 2 + PRM clb_gui_y * H);
	
	float scale = PRM clb_gui_scl * ofToFloat(PRM clb_gui_scl2);
	float scl = scale;// *H / 1080.0;
	ofScale(scl, scl, scl);

	int proj = PRM clb_proj; //XY, XZ, YZ, Custom
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
		ofRotateX(PRM clb_rotx);
		ofRotateY(PRM clb_roty);
		ofRotateZ(PRM clb_rotz);
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
	float rx = PRM clb_w / 2;
	float ry = PRM clb_h / 2;
	ofLine(-rx, 0, 0, rx, 0, 0);
	ofLine(0, -ry, 0, 0, ry, 0);
	ofLine(0, 0, 0, 0, 0, PRM clb_Z);

	ofLine(-rx, 0, PRM clb_Z, rx, 0, PRM clb_Z);
	ofLine(0, -ry, PRM clb_Z, 0, ry, PRM clb_Z);

	ofRect(-PRM clb_w / 2, -PRM clb_h / 2, PRM clb_w, PRM clb_h);
	ofLine(-PRM clb_w / 2, 0, 0, -PRM clb_w / 2, 0, PRM clb_Z);
	ofLine(PRM clb_w / 2, 0, 0, PRM clb_w / 2, 0, PRM clb_Z);
	ofLine(0, -PRM clb_h / 2, 0, 0, -PRM clb_h / 2, PRM clb_Z);
	ofLine(0, PRM clb_h / 2, 0, 0, PRM clb_h / 2, PRM clb_Z);

	ofPopMatrix();

	
}

//--------------------------------------------------------------
ofMatrix4x4 Calibrate3D::get_matrix() {
	return matrix_main_;
}

//--------------------------------------------------------------
void Calibrate3D::transform(vector<ofPoint> &points_in, vector<ofPoint> &points_out) {
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
	if (PRM clipping) {
		//cout << n << " ... ";
		float radx = PRM clb_w / 2;
		float rady = PRM clb_h / 2;
		for (int i = 0; i < n; ) {
			ofPoint &p = points_out[i];
			if (p.z < 0 || p.z > PRM clb_Z || p.x < -radx || p.x > radx || p.y < -rady || p.y > rady) {
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
void Calibrate3D::rotate(ofPoint direction, float degrees) {
	if (degrees == 0) return;
	ofMatrix4x4 m;
	m.makeRotationMatrix(degrees, direction.x, direction.y, direction.z);
	matrix_main_ *= m;
}

//--------------------------------------------------------------
void Calibrate3D::move(float x, float y, float z) {
	if (x == 0 && y == 0 && z == 0) return;
	ofMatrix4x4 m;
	m.makeTranslationMatrix(ofPoint(x,y,z));
	matrix_main_ *= m;

}

//--------------------------------------------------------------
void Calibrate3D::change_scale(float scl) {
	if (scl == 1) return;
	change_scale(scl, scl, scl);
}

//--------------------------------------------------------------
void Calibrate3D::change_scale(float sclx, float scly, float sclz) {	//for flips
	ofMatrix4x4 m;
	m.makeScaleMatrix(sclx, scly, sclz);
	matrix_main_ *= m;
}

//--------------------------------------------------------------
 
