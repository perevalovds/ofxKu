﻿#pragma once

#include "ofMain.h"

//Flood fill and blobs processing

//Rose of winds
//sv=4 or 8 - connectivity of pixels
vector<int> ofxKuRoseOfWinds(int sv, int w);

//Flood fill
//outPoints as x+w*y
size_t ofxKuFloodFill(vector<unsigned char> &input, int w, int h, int sv,
                 int x0, int y0, int search, int fillColor, vector<int> *outPoints = 0); 

//Remove small blobs, which has values [good_val0, good_val1] and area < min_area. For this blobs, set these to erase_val
//Function can work in-place, that is "output" can be "input"
void ofxKuRasterBlobsFilter(vector<unsigned char> &input, vector<unsigned char> &output, int w, int h, int sv,
							int min_area, unsigned char good_val0, unsigned char  good_val1, unsigned char erase_val );


//Blob data
struct ofxKuBlob {
	int w = 0;
	int h = 0;
	vector<int> pnt;	//x = pnt[i] % w, y = pnt[i] / w;
	int Sum = 0;			//field sum
	ofPoint center_mass(); //center of mass
	ofRectangle bbox();	//boundary box on integer grid, point has size (1,1), if blob is empty, returns width=height=-1
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
template <typename T>
void ofxKuBlobDetectInField(vector<T> &field, int w, int h, const ofxKuBlobDetectorParams &params, vector<ofxKuBlob> &blobs) {
	vector<int> rose = ofxKuRoseOfWinds(params.sv, w);

	//scan
	vector<unsigned char> mask(w*h, 1);  //TODO here is memory allocation, please declare as static to works faster
	vector<int> res;
	for (int Y = 0; Y < h; Y++) {
		for (int X = 0; X < w; X++) {
			int I = X + w * Y;
			if (field[I] > 0 && mask[I]) {
				mask[I] = 0;
				int sum = field[I];
				res.clear();
				res.push_back(I);

				size_t begin = 0;
				while (begin < res.size()) {
					int p = res[begin];
					begin++;
					for (int i = 0; i < params.sv; i++) {
						int q = p + rose[i];
						int x = q % w;
						int y = q / w;
						if (x >= 0 && x < w && y >= 0 && y < h && field[q] > 0 && mask[q]) {
							res.push_back(q);
							mask[q] = 0;
							sum += field[q];
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

