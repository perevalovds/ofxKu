#pragma once

//Working with files

#include "ofMain.h"


bool ofxKuFileExists( string fileName, bool use_data_path = true );
int ofxKuFileSize( string fileName, bool use_data_path = true);

vector<string> ofxKuFileReadStrings( string fileName, bool use_data_path = true);
string ofxKuFileReadString(string file_name, bool use_data_path = true);

bool ofxKuFileWriteStrings( const vector<string> &list, string fileName, bool use_data_path = true, bool append = false);
bool ofxKuFileWriteString(const string &line, string fileName, bool use_data_path = true, bool append = false);




//string ofxKuFileGetExtension( string fileName, bool lowerCase = true );
//vector<string> readStringsFilterAndNotEmpty( const string &fileName );
//vector<float> readFloats( const string &fileName );
//bool writeFloats( const vector<float> &list, const string &fileName );
//vector<int> readInts( const string &fileName );
//bool writeInts( const vector<int> &list, const string &fileName );
//	struct PairFloat {
//		PairFloat( const string &name0, float value0 ) {
//			name = name0;
//			value = value0;
//		}
//		string name;
//		float value;
//	};

//	static vector<PairFloat> readPairs( const string &fileName ); 
//	static void writePairs( const vector<PairFloat> &list, const string &fileName  ); 
//	static float findValue( const vector<PairFloat> &list, string name, float defValue = 0 );

//static string readOneString( const string &fileName );
//static void writeOneString( const string &fileName, const string &s );


