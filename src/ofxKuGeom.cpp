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
    normal = glm::vec2(a, b);
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
        && ofInRange(X, min(x3, x4), max(x3, x4)) && ofInRange(Y, min(y3, y4), max(y3, y4));
    
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
void ofxKuGeomLine2D::draw_rect(float rad) {	//draw quad with "rad" distance
    glm::vec2 norm = normal * rad;
    glm::vec2 con = glm::vec2(-b, a) * rad;   // conormal
    ofBeginShape();
    ofVertex(p0 - con + norm );
    ofVertex(p0 - con - norm );
    ofVertex(p1 + con - norm );
    ofVertex(p1 + con + norm );
    ofEndShape();
}

//--------------------------------------------------------
// Line equation is p0 + t*dir_unnormalized, t in [0,1], 
ofxKuGeomLine3D::ofxKuGeomLine3D(const glm::vec3& p0, const glm::vec3& p1) {
    setup(p0, p1);
}

//--------------------------------------------------------
void ofxKuGeomLine3D::setup(const glm::vec3& p0, const glm::vec3& p1) {
    this->p0 = p0;
    this->p1 = p1;
    dir_unnormalized = p1 - p0;
}

//--------------------------------------------------------
void ofxKuGeomLine3D::setup_ray(const glm::vec3& p0, const glm::vec3& p1) {
    setup(p0, p1);
    is_ray = true;
}

//--------------------------------------------------------
bool ofxKuGeomLine3D::is_t_valid(float t) const {	// t must be in [0,1] if it's line, and t>=0 for ray
    if (is_ray) return (t >= 0);
    return t >= 0 && t <= 1;
}

//--------------------------------------------------------
bool ofxKuGeomPlane::setup(const glm::vec3& origin, const glm::vec3& normal, bool normalize) {
    base = origin;
    norm = normal;
    if (normalize) {
        if (glm::length2(norm) > 0) {
            norm = glm::normalize(norm);
        }
        else {
            d = 0;
            return false;
        }
    }
    d = -glm::dot(norm, base);
    return true;
}

//--------------------------------------------------------
bool ofxKuGeomPlane::setup(const glm::vec3& origin, const glm::vec3& vec1, const glm::vec3& vec2) {
    const bool normalize = true;
    return setup(origin, glm::cross(vec1, vec2), normalize);
}

//--------------------------------------------------------
bool ofxKuGeomPlane::setup_by_points(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
    return setup(p0, p1 - p0, p2 - p0);
}

//--------------------------------------------------------
void ofxKuGeomPlane::revert_normal() {
    setup(base, -norm);
}

//--------------------------------------------------------
float ofxKuGeomPlane::signed_distance(const glm::vec3& point) const {
    return glm::dot(norm, point) + d;
}

//--------------------------------------------------------
// Projection of the point to plane
glm::vec3 ofxKuGeomPlane::projection(const glm::vec3& point) const {
    return point - norm * signed_distance(point);
}

//--------------------------------------------------------
// Crossing plane and line, if t in [0,1] it means crossed as segmen
/*
line p = a+qt
plane n*p + d = 0
n*(a+qt)+d = n*a + n*qt+d = 0
t = - (n*a+d)/(n*q),  and n*q != 0
*/
ofxKuGeomLine3D::CrossResult ofxKuGeomPlane::cross_line(const ofxKuGeomLine3D& line) const {
    float n_q = glm::dot(norm, line.dir_unnormalized);
    if (fabs(n_q) < 0.00001) {      // We are not interested in parallel case
        return ofxKuGeomLine3D::CrossResult();
    }
    ofxKuGeomLine3D::CrossResult cross;
    cross.crossed = true;
    cross.t = -(glm::dot(norm, line.p0) + d) / n_q;
    cross.p = line.p0 + line.dir_unnormalized * cross.t;
    return cross;
}


//--------------------------------------------------------
ofxKuGeomTriangle3D::ofxKuGeomTriangle3D(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
    setup(p0, p1, p2);
}

//--------------------------------------------------------
void ofxKuGeomTriangle3D::setup(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    plane.setup_by_points(p0, p1, p2);

    plane01.setup(p0, glm::cross(plane.norm, p1 - p0));
    plane12.setup(p0, glm::cross(plane.norm, p2 - p1));
    plane20.setup(p0, glm::cross(plane.norm, p0 - p2));
    glm::vec3 M = (p0 + p1 + p2) / 3;
    if (plane01.signed_distance(M) < 0) 
        plane01.revert_normal();
    if (plane12.signed_distance(M) < 0)
        plane12.revert_normal();
    if (plane20.signed_distance(M) < 0)
        plane20.revert_normal();
}

//--------------------------------------------------------
// Crossing triangle and line, if t in [0,1] it means crossed as segment
/*
1. Cross line and triangle's plane
2. Create 3 planes at side,
and check if p is inside each of the half-space limited by plane
*/
ofxKuGeomLine3D::CrossResult ofxKuGeomTriangle3D::cross_line(const ofxKuGeomLine3D& line) const {
    // Check triangle's plane crosses segment
    ofxKuGeomLine3D::CrossResult cross = plane.cross_line(line);
    bool result = (cross.crossed && line.is_t_valid(cross.t));

    // Check crossing point is inside triangle
    if (result) {
        result = plane01.signed_distance(cross.p) >= 0
            && plane12.signed_distance(cross.p) >= 0
            && plane20.signed_distance(cross.p) >= 0;
    }
    if (!result) {
        return ofxKuGeomLine3D::CrossResult();
    }
    return cross;
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
