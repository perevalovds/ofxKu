#include "ofxKuGeom.h"

//--------------------------------------------------------
ofxKuGeomLine2D::ofxKuGeomLine2D(const ofPoint &p0, const ofPoint &p1) {
	setup(p0, p1);
}

//--------------------------------------------------------
void ofxKuGeomLine2D::setup(const ofPoint &p0, const ofPoint &p1) {
	this->p0 = p0; this->p1 = p1;
	a = p1.y - p0.y;
	b = p0.x - p1.x;
	//normalize
	float len = a * a + b * b;
	if (len > 0) {
		len = sqrt(len);
		a /= len;
		b /= len;
	}
	c = -a * p0.x - b * p0.y;
}

//--------------------------------------------------------
//signed distance
float ofxKuGeomLine2D::sgd(const ofPoint &p) {
	return p.x * a + p.y * b + c;
}

//--------------------------------------------------------
ofPoint ofxKuGeomLine2D::project(const ofPoint &p) {	//project point onto line
	float t = sgd(p);
	return p - ofPoint(a, b)*t;
}

//--------------------------------------------------------
ofPoint ofxKuGeomLine2D::mirror(const ofPoint &p) {	//mirror
	float t = sgd(p);
	return p - ofPoint(a, b)*(2*t);
}

//--------------------------------------------------------

//--------------------------------------------------------
//Minimal distance between two point sets
float ofxKuPointCloudsDistance(const vector<ofPoint> &A, const vector<ofPoint> &B) {
	if (A.empty() || B.empty()) return 0;
	float d = A[0].distanceSquared(B[0]);
	for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < B.size(); j++) {
			d = min(d, A[i].distanceSquared(B[j]));
		}
	}
	return sqrt(fabs(d));
}


//--------------------------------------------------------

//Convert [0,1]x[0,1] -> Unit Sphere
ofPoint ofxKuGeomSquareToSphere(const ofPoint &p2) {
    float a = p2.x*TWO_PI;
    float b = p2.y*PI;
    ofPoint p3;
    p3.x = sin(b) * cos(a);
    p3.y = sin(b) * sin(a);
    p3.z = cos(b);
    return p3;
}


//--------------------------------------------------------------
ofPoint ofxKuGeomSphereToSquare(const ofPoint &p3) {
    float b = acos(p3.z);
    float a = 0;
    if (p3.y != 0) {
        a = atan2(p3.y, p3.x);
    }
    if (a < 0) a += TWO_PI;
    return ofPoint(a / TWO_PI, b / PI);
}

//--------------------------------------------------------------
float ofxKuGeomTriangleAreaSigned(const ofPoint &a, const ofPoint &b, const ofPoint &c) {
    return (b-a).getCrossed(c-a).z/2;
}

//--------------------------------------------------------------
bool ofxKuGeomPointInsidePolygonCcvStrictly(const ofPoint &p, const vector<ofPoint> &pnt) {
    int N = pnt.size();
    if (N < 3) return false;
    
    int sign = 0;
    for (int i=0; i<N; i++) {
        const ofPoint &a = pnt[i];
        const ofPoint &b = pnt[(i+1) % N];
        float t = ofxKuGeomTriangleAreaSigned(p, a, b);
        if (t >= 0) return false;
    }
    return true;
}


//--------------------------------------------------------------
//Generate a circle on a unit sphere
vector<ofPoint> ofxKuGeomCreateCircleOnSphere(ofPoint c3, float rad, int resolution) {
    int Res = resolution;
    
    //create coordinate system
    ofPoint OX(1, 0, 0);
    ofPoint axe1 = c3;
    ofPoint axe2 = OX - axe1 * OX.dot(axe1);
    if (axe2.length() > 0) {
        axe2.normalize();
    }
    else {
        ofPoint OY(0, 1, 0);
        axe2 = OY - axe1 * OY.dot(axe1);
        axe2.normalize();
    }
    ofPoint axe3 = axe1.getCrossed(axe2);
    axe3.normalize(); //though is not required
    
    
    //Make points
    //rad 0..PI/2 = angle
    float ang = min(rad * TWO_PI, PI/2);        //TODO check for errors !!!!!!!!!!!!!!!!
    float DX = cos(ang);
    float DY = sin(ang);
    
    vector<ofPoint> p3(Res);
    for (int i = 0; i < Res; i++) {
        float a = i * TWO_PI / Res;
        float x = cos(a);
        float y = sin(a);
        
        p3[i] = axe1 * DX + axe2 * (x*DY) + axe3 * (y*DY);
        p3[i].normalize();
    }
    
    return p3;
}


//--------------------------------------------------------
