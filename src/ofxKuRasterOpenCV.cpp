#include "ofxKuRasterOpenCV.h"
#include "ofxOpenCv.h"
#include "ofxKuRaster.h"
#include "tracking.hpp"

//-------------------------------------------------------------
void ofxKuRasterGaussSmooth( vector<float> &mask, int w, int h, int rad, vector<float> &res )
{
    ofxCvFloatImage img;
	img.setUseTexture(false);
    img.allocate( w, h );
    img.setFromPixels( &mask[0], w, h );
    
    img.blurGaussian( 2 * rad + 1 );
    
    float *imgF = img.getPixelsAsFloats();
    res.resize( w * h );
    for (int i=0; i<w*h; i++) {
        res[i] = imgF[i];
    }
    
}

//-------------------------------------------------------------
void ofxKuRasterResized( vector<float> &mask, int w, int h, int rad, vector<float> &res, int wres, int hres )
{
    ofxCvFloatImage img;
	img.setUseTexture(false);
    img.allocate( w, h );
    img.setFromPixels( &mask[0], w, h );
    img.resize( wres, hres );
    
    img.blurGaussian( 2 * rad + 1 );
    
    float *imgF = img.getPixelsAsFloats();
    res.resize( wres * hres );
    for (int i=0; i<wres*hres; i++) {
        res[i] = imgF[i];
    }
    
}

//-------------------------------------------------------------
void ofxKuRasterFieldDirection( vector<float> &energy, int w, int h, int step, vector<ofPoint> &res )
{
    res.resize( w*h );
    for (int i=0; i<w*h; i++) {
        res[i].x = res[i].y = 0;
    }
    
    float EPS = 0.000001;
    float x0, x1, y0, y1;
    ofPoint dir;
    for (int y=step; y<h-step; y++) {
        for (int x=step; x<w-step; x++) {
            x0 = energy[ x-step + w * y ];
            x1 = energy[ x+step + w * y ];
            y0 = energy[ x + w * (y-step) ];
            y1 = energy[ x + w * (y+step) ];
            if ( x0 > EPS && x1 > EPS && y0 > EPS && y1 > EPS ) {
                dir.x = x1 - x0;
                dir.y = y1 - y0;
                //dir.normalize();
                res[x + w*y] = dir;
            }
        }
    }
}

//-------------------------------------------------------------
//Заполняет нулевые направления в accum значениями из source
/*void pbCombineFieldDirection( vector<ofPoint> &accum, int w, int h, vector<ofPoint> &source, int ws, int hs  )
{
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            ofPoint &a = accum[x+w*y];
            if ( a.x == 0 && a.y == 0 ) {
                a = source[ x * ws / w + ws*( y * hs / h) ];
            }
        }
    }
}
*/

//-------------------------------------------------------------
//Заполняет нулевые направления, а также граничные, направлением в center
/*void pbFillFieldDirection( vector<ofPoint> &accum, int w, int h, ofPoint &center, int border  )
{
    ofPoint dir;
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            ofPoint &a = accum[x+w*y];
            if ( //( a.x == 0 && a.y == 0 )
                //||
                (x < border ) || ( x > w-1-border) || ( y < border ) || (y > h-1-border ) ) {
                dir = center - ofPoint( x, y );
                dir.normalize();
                a = dir;
            }
        }
    }
}
*/
//-------------------------------------------------------------
/*void ofxKuRasterConvexHull( vector<unsigned char> &input, vector<unsigned char> &output, int w, int h, int threshold ) {

	output.resize(w*h);
	fill(output.begin(), output.end(), 0);

	//contour points
	vector<CvPoint> contour;
	ofxKuRasterBoundaryPoints(input, contour, w, h, (unsigned char)(threshold));
	
	//debug draw contour
	//for (int i=0; i<contour.size(); i++) {
	//	int x = contour[i].x;
	//	int y = contour[i].y;
	//	output[x+w*y] = 255;
	//}

	//convex hull
	int n = contour.size();
	if (n>0) {
		vector<int> hull(n);
		int hullsize = n;
		cvConvexHull( &contour[0], n, 0, CV_COUNTER_CLOCKWISE, &hull[0], &hullsize );

		//debug draw
		//for (int i=0; i<hullsize; i++) {
		//	int x = contour[hull[i]].x;
		//	int y = contour[hull[i]].y;
		//	output[x+w*y] = 255;
		//}

		//get convex contour
		vector<CvPoint> convex(hullsize);
		for (int i=0; i<hullsize; i++) {
			convex[i] = contour[hull[i]];
		}

		//draw to output
		ofxCvGrayscaleImage img;
		img.setUseTexture(false);
		img.allocate( w, h );
		img.set(0);
		cvFillConvexPoly(img.getCvImage(), &convex[0], hullsize, cvScalar(255));
    
		unsigned char *imgData = img.getPixels();
		for (int i=0; i<w*h; i++) {
			output[i] = imgData[i];
		}	
	}
}
*/

//-------------------------------------------------------------
void ofxKuOpticalFlowFarneback(ofPixels &pix_gray1, ofPixels &pix_gray2, float scale, vector<ofVec2f> &flow_out, int &w, int &h) {
	ofxCvGrayscaleImage gray1_, gray2_;
	gray1_.setUseTexture(false);
	gray2_.setUseTexture(false);
	int ww = pix_gray1.getWidth();
	int hh = pix_gray1.getHeight();
	gray1_.allocate(ww, hh);
	gray2_.allocate(ww, hh);
	gray1_.setFromPixels(pix_gray1);
	gray2_.setFromPixels(pix_gray2);

	ofxCvGrayscaleImage gray1, gray2;
	gray1.setUseTexture(false);
	gray2.setUseTexture(false);

	gray1.allocate(gray1_.width * scale, gray1_.height * scale);
	gray2.allocate(gray1_.width * scale, gray1_.height * scale);

	gray1.scaleIntoMe(gray1_, CV_INTER_AREA);
	gray2.scaleIntoMe(gray2_, CV_INTER_AREA);

	//imageDecimated.allocate(color1.width * scale_input, color1.height * scale_input);
	//imageDecimated.scaleIntoMe(color1, CV_INTER_AREA);
	//gray1 = imageDecimated;

	w = gray1.width;
	h = gray1.height;

	//TODO check w,h>0

	cv::Mat img1 = cv::cvarrToMat(gray1.getCvImage());
	cv::Mat img2 = cv::cvarrToMat(gray2.getCvImage());
	
	cv::Mat flow;
	//Calling native OpenCV function for computing optical flow
	cv::calcOpticalFlowFarneback(img1, img2, flow, 0.7, 3, 11, 5, 5, 1.1, 0);
	vector<cv::Mat> flowPlanes;
	cv::split(flow, flowPlanes);

	ofxCvFloatImage flowX, flowY;

	IplImage iplX(flowPlanes[0]);
	flowX.allocate(w, h);
	flowX = &iplX;
	IplImage iplY(flowPlanes[1]);
	flowY.allocate(w, h);
	flowY = &iplY;

	//read to vector array
	float *flowXFloats = flowX.getPixelsAsFloats(); 
	float *flowYFloats = flowY.getPixelsAsFloats(); 

	flow_out.resize(w*h);
	for (int i = 0; i < w*h; i++) {
		flow_out[i] = ofVec2f(flowXFloats[i], flowYFloats[i]);
	}


}



//-------------------------------------------------------------
