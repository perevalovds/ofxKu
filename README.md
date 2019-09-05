# ofxKu
Collection of various useful modules for openFrameworks which I used in various projects for a last years.

It contains classes and functions for graphics, image analysis and working with files.
* Modules are relatively independent. 
* Module ofxKuRasterOpenCV requires ofxOpenCv addon (if you don't need it, just not include in your project).
* Module ofxKuMessageLog requires ofxFontStash addon (if you don't need it...).
* Module ofxKuOsWindows works only in Windows (if you don't need it...).
* ofxKuFileTime::isNewerThan function implementation doesn't works in oF >= 0.9.0, so simplest workaround is change it to return always 'true'.

## Compression and hash
  ofxKuCompress - RLE and bit compression
  ofxKuHashCrc32 - computing CRC32 hash

## Strings
  ofxKuStrings
   * convert matrix to string and back
   * convert vector<string> to vector<int>, vector<float> and back
   * ofSplitString_many - advanced version of ofSplitString, which allows to use many one-char delimiters.
   For example, this funtion allows to parse effectively "a(b,c)" with delimiters "(,)"

## Drawing
* ofxKuRectRender class for fast rendering rectangles
* ofxKuLineRender class for fast rendering lines
* ofxKuVertexRender class for fast rendering points
* ofxKuDrawTextureFit(ofTexture &tex, float x, float y, float w, float h) function in ofxKuDrawUtils for fitting texture in rectangle
* ofxKuDrawRaster draws raster array

## Geometry, meshes and point clouds
1. ofxKuGeom - geometrical utils
   * Point clouds:
      * minimal distance between two point sets
   * Polygons:
      * signed triangle area
      * compute if point is inside polygon
   * Sphere:
      * convert points from unit square to unit sphere and back
      * create circle on a unit sphere
   

2. ofxKuGeomMeshUtils - utilities for mesh
   * loading OBJ files
   * computing normals
   * make wireframed mesh from triangle mesh
   * shuffle verties and triangles

3. Point clouds:
   * ofxKuPcManipulate - move, rotate and crop point clouds
   * ofxKuPcRasterize - rasterize and get blobs from point clouds
   * also see ofxKuGeom for point sets operations

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
* ofxKuMathStat - computes mean and median index of histogram value and argmax

## Values
* ofxKuValueCheckChange - checks if value was changed
* ofxKuSlowBit - bit, which controllable changes it state in time

## Presets
* ofxKuPreset - presets maintaining system, including smooth transition,
also can be linked with ofxKuTextGui GUI system.

## File and system utilities
* ofxKuRecorderBinary class for recording/replaying data buffers, such as depth camera point clouds
* ofxKuRecorderStrings class for storing string data in frames in a text file
* ofxKuFile - write and read text files as vector of strings
* ofxKuFileTime::isNewerThan(string fileName1, string fileName2) function which compares file creation times
* ofxIniSettings - addon for working with INI-files, by Rick Companje
* ofxArgs - addon for working with command line arguments, by Pat Long

## OS-specific
* ofxKuOsWindows conta ins Windows-specific functions for controlling OpenGL windows (make borderless, set position and size for borderless fullscreen) 
and console window (hide,show).

## Logging
* ofxKuMessageLog - class for output logs on the screen. Note: requires ofxFontStash addon for drawing text on the screen.