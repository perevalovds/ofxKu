#include "ofxKuGeom.h"

//--------------------------------------------------------
ofxKuGeomLine2D::ofxKuGeomLine2D(const glm::vec2 &p0, const glm::vec2 &p1) {
	setup(p0, p1);
}

//--------------------------------------------------------
ofxKuGeomLine2D::ofxKuGeomLine2D(float x1, float y1, float x2, float y2) {
    setup(glm::vec2(x1, y1), glm::vec2(x2, y2));
}

//--------------------------------------------------------
void ofxKuGeomLine2D::setup(const glm::vec2 &p0, const glm::vec2 &p1) {
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
float ofxKuGeomLine2D::sgd(const glm::vec2 &p) {
	return p.x * a + p.y * b + c;
}

//--------------------------------------------------------
glm::vec2 ofxKuGeomLine2D::project(const glm::vec2 &p) {	//project point onto line
	float t = sgd(p);
	return p - ofPoint(a, b)*t;
}

//--------------------------------------------------------
glm::vec2 ofxKuGeomLine2D::mirror(const glm::vec2 &p) {	//mirror
	float t = sgd(p);
	return p - ofPoint(a, b)*(2*t);
}

//--------------------------------------------------------
//if pcross is not NULL, sets to crossing point
//note: we don't consider boundary cases such as parallel lines
bool ofxKuGeomLine2D::intersect_segments(const ofxKuGeomLine2D &line, glm::vec2 *pcross) {
    //https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    const float &x1 = p0.x;
    const float &y1 = p0.y;
    const float& x2 = p1.x;
    const float& y2 = p1.y;
    const float& x3 = line.p0.x;
    const float& y3 = line.p0.y;
    const float& x4 = line.p1.x;
    const float& y4 = line.p1.y;

    float D = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (fabs(D) < 0.00001) {    //TODO parameters EPS
        return false;
    }

    float D1 = x1 * y2 - y1 * x2;
    float D2 = x3 * y4 - y3 * x4;
    float X = (D1 * (x3 - x4) - (x1 - x2) * D2) / D;
    float Y = (D1 * (y3 - y4) - (y1 - y2) * D2) / D;

    //check that point is inside segments
    bool inside = ofInRange(X, min(x1, x2), max(x1, x2)) && ofInRange(Y, min(y1, y2), max(y1, y2))
        && ofInRange(X, min(x1, x2), max(x1, x2)) && ofInRange(Y, min(y1, y2), max(y1, y2));
    
    if (!inside) {
        return false;
    }
    if (pcross) {
        pcross->x = X;
        pcross->y = Y;
    }

    return true;

}

//--------------------------------------------------------
bool ofxKuGeomLine2D::intersect_segments(const glm::vec2& p0, const glm::vec2& p1, glm::vec2* pcross) {
    ofxKuGeomLine2D line(p0, p1);
    return intersect_segments(line, pcross);
}


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
