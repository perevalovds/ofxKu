# ofxKu
My extension for openFrameworks.

It contains classes and functions for rendering, image analysis and working with files, which we are using in Kuflex projects.
* Modules are relatively independent. 
* Module ofxKuRasterOpenCV requires ofxOpenCv addon to work - if you don't need it, kust not include in your project.
* ofxKuFileTime::isNewerThan function implementation doesn't works in oF >= 0.9.0, te simplest workaround is change it to return always 'true'.

## Compression
  ofxKuCompress - RLE and bit compression

## Converting
  ofxKuConvertTypes
   * convert matrix to string and back
   * convert vector<string> to vector<int>, vector<float> and back


## Drawing
* ofxKuRectRender class for fast rendering rectangles
* ofxKuLineRender class for fast rendering lines
* ofxKuVertexRender class for fast rendering points
* ofxKuDrawTextureFit(ofTexture &tex, float x, float y, float w, float h) function in ofxKuDrawUtils for fitting texture in rectangle
* ofxKuDrawRaster draws raster array

## Geometry and meshes
1. ofxKuGeom - geometrical utils
   * signed triangle area
   * compute if point is inside polygon
   * convert points from unit square to unit sphere and back
   * create circle on a unit sphere
   

2. ofxKuGeomMeshUtils - utilities for mesh
   * loading OBJ files
   * computing normals
   * make wireframed mesh from triangle mesh
   * shuffle verties and triangles

## Graphics
* ofxKuGraphicsTexture3D - class implementing 3D GPU texture for programmable rendering pipeline
   

## Image processing
* ofxKuRaster.h - include it for accessing all image processing functions
* ofxKuRasterArithmetics - add, subtract, multiply, divide, threshold, find mask for range values, 
count number of pixels in range, project on axes, histogram for raster arrays
* ofxKuRasterBlur_int, ofxKuRasterBlur_float - box blur for raster arrays
* ofxKuRasterInterpolate - interpolation of raster arrays, including ofPoint rasters
* ofxKuRasterMorphology - dilatation and erosion, contour detection
* ofxKuRasterResize_nearest - resize raster arrays
* ofxKuFloodFill function which do flood fill in array, removing small blobs
* ofxKuRasterOpenCV - gaussian smoothing, convex hull, implemented using OpenCV. Requires ofxOpenCv addon.

## Statistics
* ofxKuMathStat - compute median value of the vector

## Values
* ofxKuValueCheckChange - checks if value was changed
* ofxKuSlowBit - bit, controllable changing it state

## Presets
* ofxKuPreset - presets maintaining system, including smooth transition,
also can be linked with ofxKuTextGui GUI system.

## File utilities
* ofxKuRecorderBinary class for recording/replaying data buffers, such as depth camera point clouds
* ofxKuRecorderStrings class for storing string data in frames in a text file
* ofxKuFile - write and read text files as vector of strings
* ofxKuFileTime::isNewerThan(string fileName1, string fileName2) function which compares file creation times


