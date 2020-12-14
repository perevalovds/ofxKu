#pragma once

#include "ofMain.h"
#include "int2.h"

//Flood fill and blobs processing

//Rose of winds
//sv=4 or 8 - connectivity of pixels
vector<int2> ofxKuRoseOfWinds(int sv, int w);

//Flood fill
//outPoints as x+w*y
size_t ofxKuFloodFill(vector<unsigned char> &input, int w, int h, int sv,
                 int x0, int y0, int search, int fillColor, vector<int2> *outPoints = 0); 

//Remove small blobs, which has values [good_val0, good_val1] and area < min_area. For this blobs, set these to erase_val
//Function can work in-place, that is "output" can be "input"
void ofxKuRasterBlobsFilter(vector<unsigned char> &input, vector<unsigned char> &output, int w, int h, int sv,
							int min_area, unsigned char good_val0, unsigned char  good_val1, unsigned char erase_val );


//Blob data
struct ofxKuBlob {
	int w = 0;
	int h = 0;
	vector<int2> pnt;	
	int Sum = 0;			//field sum
	ofPoint center_mass(); //center of mass
	ofRectangle bbox();	//scans all points, so takes time to compute! boundary box on integer grid, point has size (1,1), if blob is empty, returns width=height=-1
	float rad(ofPoint center);
	void draw_to_raster(vector<unsigned char> &raster, unsigned char value);
};


//Parameters for searching blobs
struct ofxKuBlobDetectorParams {
	int sv;			//4 or 8 - connectivity
	int min_blob_area, max_blob_area;		//-1 means not use this restriction
	int min_count_in_blob, max_count_in_blob; //-1 means not use this restriction
	ofxKuBlobDetectorParams(int sv_, int min_blob_area_=-1, int max_blob_area_=-1,
		int min_count_in_blob_=-1, int max_count_in_blob_=-1) {
		sv = sv_;
		min_blob_area = min_blob_area_;
		max_blob_area = max_blob_area_;
		min_count_in_blob = min_count_in_blob_;
		max_count_in_blob = max_count_in_blob_;
	}
};


//-------------------------------------------------------------------------------------
//Find blobs with area in the given range and also sum(field values over blob) in the given range
//T - an integer type, such as unsigned char or int
//roix,y,w,h - if specified, only this area is used for blob search
template <typename T>
void ofxKuBlobDetectInField(vector<T> &field, int w, int h, const ofxKuBlobDetectorParams &params, vector<ofxKuBlob> &blobs,
	int roix = -1, int roiy = -1, int roiw = -1, int roih = -1) {
	vector<int2> rose = ofxKuRoseOfWinds(params.sv, w);

	if (roix == -1) {
		roix = 0;
		roiy = 0;
		roiw = w;
		roih = h;
	}
	roiw = roix + roiw;	//We are using this trick to not to use "+" below.
	roih = roiy + roih;

	//scan
	vector<unsigned char> mask(w*h, 1);  //TODO here is memory allocation, please declare as static to works faster
	vector<int2> res;
	for (int Y = roiy; Y < roih; Y++) {
		for (int X = roix; X < roiw; X++) {
			int I = X + w * Y;
			if (field[I] > 0 && mask[I]) {
				mask[I] = 0;
				int sum = field[I];
				res.clear();
				res.push_back(int2(X,Y));

				size_t begin = 0;
				while (begin < res.size()) {
					int2 p = res[begin];
					begin++;
					for (int i = 0; i < params.sv; i++) {
						int2 q = p + rose[i];
						if (q.x >= roix && q.x < roiw && q.y >= roiy && q.y < roih) {
							int qi = q.x + w * q.y;
							if (field[qi] > 0 && mask[qi]) {
								res.push_back(q);
								mask[qi] = 0;
								sum += field[qi];
							}
						}
					}
				}
				if ((res.size() >= params.min_blob_area || params.min_blob_area == -1)
					&& (res.size() <= params.max_blob_area || params.max_blob_area == -1)
					&& (sum >= params.min_count_in_blob || params.min_count_in_blob == -1)
					&& (sum <= params.max_count_in_blob || params.max_count_in_blob == -1)
					) {
					blobs.push_back(ofxKuBlob());
					ofxKuBlob &blob = blobs[blobs.size() - 1];
					blob.w = w;
					blob.h = h;
					blob.pnt = res;
					blob.Sum = sum;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------

