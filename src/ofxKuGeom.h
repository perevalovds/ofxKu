#pragma once

/*
ofxKuGeom - geometrical utils
Structure ofxKuGeomLine2D for working with line in 2D:
 - compute line equation,
 - compute signed distance from a line,
 - project point on line,
 - mirror point of line.

Triangle in 3D:
 - cross line segment and triangle 

Plane in 3D:
 - signed distance from point to plane
 - projection of the point to plane
 - cross line segment and plane

Point clouds:
 - minimal distance between two point sets
Polygons:
 - signed triangle area
 - compute if point is inside polygon
Sphere:
 - convert points from unit square to unit sphere and back
 - create circle on a unit sphere
*/

#include "ofMain.h"

// Line in 2D
struct ofxKuGeomLine2D {
	glm::vec2 p0, p1;
	glm::vec2 normal;
	//(a,b) = normal, looks on the right in left-handed coord.system (oF screen)
	//normal is normalized
	// conormal directed as p1-p0 is (-b,a)
	float a = 0;
	float b = 0;
	float c = 0;
	ofxKuGeomLine2D() {}
	ofxKuGeomLine2D(const glm::vec2 &p0, const glm::vec2 &p1);
	ofxKuGeomLine2D(float x1, float y1, float x2, float y2);
	void setup(const glm::vec2 &p0, const glm::vec2 &p1);

	float sgd(const glm::vec2 &p);		//signed distance to point
	glm::vec2 project(const glm::vec2 &p);	//project point onto line
	glm::vec2 mirror(const glm::vec2 &p);	//mirror point 

	//if pcross is not NULL, sets to crossing point
	//note: we don't consider boundary cases such as parallel lines
	bool intersect_segments(const ofxKuGeomLine2D &line, glm::vec2 *pcross = NULL); 
	bool intersect_segments(const glm::vec2& p0, const glm::vec2& p1, glm::vec2* pcross = NULL);

	void draw_rect(float rad = 0.5);	//draw quad with "rad" distance for creating hi-res shots
};

// Line in 3D
struct ofxKuGeomLine3D {
	glm::vec3 p0, p1;
	glm::vec3 dir_unnormalized;
	bool is_ray = false;	// notification that it's ray from p0

	// Line equation is p0 + t*dir_unnormalized, t in R; for segment t in [0,1]
	ofxKuGeomLine3D() {}
	ofxKuGeomLine3D(const glm::vec3& p0, const glm::vec3& p1);
	void setup(const glm::vec3& p0, const glm::vec3& p1);
	void setup_ray(const glm::vec3& p0, const glm::vec3& p1);

	bool is_t_valid(float t) const;	// t must be in [0,1] if it's line, and t>=0 for ray

	struct CrossResult {
		bool crossed = false;
		glm::vec3 p = glm::vec3(0,0,0);
		float t = 0;	// t in [0,1] means that point inside a segment [p0, p1]
	};
};


// Plane in 3D
struct ofxKuGeomPlane {
	bool setup(const glm::vec3& origin, const glm::vec3& normal, bool normalize = true);
	bool setup(const glm::vec3& origin, const glm::vec3& vec1, const glm::vec3& vec2);
	bool setup_by_points(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	void revert_normal();

	// Signed distance from point to plane
	float signed_distance(const glm::vec3& point) const;

	// Projection of the point to plane
	glm::vec3 projection(const glm::vec3& point) const;

	// Crossing plane and line, if t in [0,1] it means crossed as segment
	ofxKuGeomLine3D::CrossResult cross_line(const ofxKuGeomLine3D& line) const;

	glm::vec3 base;
	glm::vec3 norm;
	float d = 0;	//dot(norm,p) + d = 0 - equation of the plane
};

// Triangle in 3D
struct ofxKuGeomTriangle3D {
	ofxKuGeomTriangle3D() {}
	ofxKuGeomTriangle3D(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	void setup(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

	// Crossing triangle and line, if t in [0,1] it means crossed as segment
	/* Usage:
		ofxKuGeomLine3D line(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10));
		ofxKuGeomTriangle3D tri(glm::vec3(-1, -1, 1), glm::vec3(0, 1, 1), glm::vec3(1, -1, 1));
		auto cross = tri.cross_line(line);
		cout << "crossed " << cross.crossed << " t " << cross.t << "   point " << cross.p.x << ", " << cross.p.y << ", " << cross.p.z << endl;
	*/
	ofxKuGeomLine3D::CrossResult cross_line(const ofxKuGeomLine3D& line) const;

	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	ofxKuGeomPlane plane, plane01, plane12, plane20;
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







