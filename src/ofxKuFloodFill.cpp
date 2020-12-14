#include "ofxKuFloodFill.h"
#include "ofxKuRasterArithmetics.h"
//#include "ofxKuRaster.h"

//--------------------------------------------------------------
//sv=4 or 8 - connectivity of pixels
vector<int2> ofxKuRoseOfWinds(int sv, int w) {
	vector<int2> rose(sv);
	rose[0] = int2(1,0);
	rose[1] = int2(-1,0);
	rose[2] = int2(0,1);
	rose[3] = int2(0,-1);
	if (sv == 8) {
		rose[4] = int2(1,1);
		rose[5] = int2(1,-1);
		rose[6] = int2(-1,1);
		rose[7] = int2(-1,-1);
	}
	return rose;
}

//--------------------------------------------------------------
size_t ofxKuFloodFill(vector<unsigned char> &input, int w, int h, int sv,
                 int x0, int y0, int search, int fillColor, vector<int2> *outPoints) {
	vector<int2> *res = (outPoints)?outPoints:(new vector<int2>());
	res->clear();
	if (input[x0+w*y0] != search) return 0;
    res->push_back(int2(x0,y0));
    input[x0 + w*y0] = fillColor;

	vector<int2> rose = ofxKuRoseOfWinds(sv, w);

    size_t begin = 0;
    while (begin < res->size()) {
        int2 p = (*res)[begin];
        begin++;
		for (int i=0; i<sv; i++) {
			int2 q = p+rose[i];			
			if (q.x >= 0 && q.x < w && q.y >= 0 && q.y < h && input[q.x+w*q.y] == search) {
				res->push_back(q);
				input[q.x+w*q.y] = fillColor;
			}
		}
    }
	int n = res->size();
    if ( !outPoints ) { delete res; }
    return n;
}

//--------------------------------------------------------------
//remove small blobs, which has values [good_val0, good_val1] and area < min_area. For this blobs, set these to erase_val
void ofxKuRasterBlobsFilter(vector<unsigned char> &input, vector<unsigned char> &output, int w, int h, int sv,
							int min_area, unsigned char good_val0, unsigned char good_val1, unsigned char erase_val ) {
	output = input;
	vector<unsigned char> mask;
	ofxKuRasterRangeMask(input, mask, w, h, good_val0, good_val1, (unsigned char) (0), (unsigned char) (255) );
	vector<int2> fillPoints;
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			if (mask[x+w*y] == 255) {
				int area = ofxKuFloodFill(mask, w, h, sv, x, y, 255, 128, &fillPoints);
				if (area < min_area) {
					//erasing small area
					for (int i=0; i<fillPoints.size(); i++) {
						int2 p = fillPoints[i];
						output[p.x + w*p.y] = 0;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
ofPoint ofxKuBlob::center_mass() {
	double X = 0;
	double Y = 0;
	int n = pnt.size();
	for (int i = 0; i < n; i++) {
		X += pnt[i].x;
		Y += pnt[i].y;
	}
	if (n) {
		X /= n;
		Y /= n;
	}
	return ofPoint(X, Y);
}

//--------------------------------------------------------------
glm::vec2 ofxKuBlob::center_mass2() { //center of mass
	auto p = center_mass();
	return glm::vec2(p.x, p.y);
}

//--------------------------------------------------------------
ofRectangle ofxKuBlob::bbox() {	//boundary box
	if (pnt.empty()) {
		return ofRectangle(0, 0, -1, -1);
	}
	int x0 = pnt[0].x;
	int y0 = pnt[0].y;
	int x1 = x0;
	int y1 = y0;
	for (int i = 0; i < pnt.size(); i++) {
		int x = pnt[i].x;
		int y = pnt[i].y;
		x0 = min(x0, x);
		y0 = min(y0, y);
		x1 = max(x1, x);
		y1 = max(y1, y);
	}
	return ofRectangle(x0, y0, x1 + 1 - x0, y1 + 1 - y0);
}

//--------------------------------------------------------------
float ofxKuBlob::rad(ofPoint center) {
	float R = 0;
	int n = pnt.size();
	for (int i = 0; i < n; i++) {
		const int x = pnt[i].x;
		const int y = pnt[i].y;
		R = max(R, ofDist(x, y, center.x, center.y));
	}
	return R;
}

//--------------------------------------------------------------
void ofxKuBlob::draw_to_raster(vector<unsigned char> &raster, unsigned char value) {
	int n = pnt.size();
	for (int i = 0; i < n; i++) {
		const int x = pnt[i].x;
		const int y = pnt[i].y;
		raster[x + w*y] = value;
	}
}

//--------------------------------------------------------------
