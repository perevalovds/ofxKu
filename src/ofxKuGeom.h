#pragma once

//ofxKuGeom - geometrical utils
//Structure ofxKuGeomLine2D for working with line in 2D:
// - compute line equation,
// - compute signed distance from a line,
// - project point on line,
// - mirror point of line.

//Point clouds:
// - minimal distance between two point sets
//Polygons:
// - signed triangle area
// - compute if point is inside polygon
//Sphere:
// - convert points from unit square to unit sphere and back
// - create circle on a unit sphere

#include "ofMain.h"

struct ofxKuGeomLine2D {
	ofPoint p0, p1;
	//(a,b) looks on the right in left-handed coord.system (oF screen)
	float a = 0;
	float b = 0;
	float c = 0;
	ofxKuGeomLine2D() {}
	ofxKuGeomLine2D(const ofPoint &p0, const ofPoint &p1);
	void setup(const ofPoint &p0, const ofPoint &p1);

	float sgd(const ofPoint &p);		//signed distance
	ofPoint project(const ofPoint &p);	//project point onto line
	ofPoint mirror(const ofPoint &p);	//mirror

};

//Minimal distance between two point sets
float ofxKuPointCloudsDistance(const vector<ofPoint> &A, const vector<ofPoint> &B);

//Triangle area, signed, 2D
float ofxKuGeomTriangleAreaSigned(const ofPoint &a, const ofPoint &b, const ofPoint &c);

//Check if point is strictly inside polygon, given CCV
bool ofxKuGeomPointInsidePolygonCcvStrictly(const ofPoint &p, const vector<ofPoint> &pnt);


//Convert unit square [0,1]x[0,1] to unit sphere
ofPoint ofxKuGeomSquareToSphere(const ofPoint &p2);

//Convert unit sphere to unit square [0,1]x[0,1]
ofPoint ofxKuGeomSphereToSquare(const ofPoint &p3);


//Generate a circle on a unit sphere
vector<ofPoint> ofxKuGeomCreateCircleOnSphere(ofPoint c3, float rad, int resolution);







