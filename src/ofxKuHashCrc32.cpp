#include "ofxKuHashCrc32.h"
#include "ofxKuFile.h"

//--------------------------------------------------------------
unsigned int ofxKuHashCrc32(void *buffer, unsigned long len)
{
	unsigned long crc_table[256];
	unsigned long crc;
	for (int i = 0; i < 256; i++)
	{
		crc = i;
		for (int j = 0; j < 8; j++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
		crc_table[i] = crc;
	};
	crc = 0xFFFFFFFFUL;

	unsigned char *buf = (unsigned char*)buffer;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFFUL;
}

//--------------------------------------------------------------
unsigned int ofxKuHashCrc32(const std::string &file_name, bool use_data_path) {
	unsigned long len = ofxKuFileSize(file_name, use_data_path);
	vector<char> buffer(len);
	if (len > 0) {
		string file1 = file_name;
		if (use_data_path) file1 = ofToDataPath(file1);
		ifstream f(file1, std::ios::binary);
		f.read(&buffer[0], len);
	}
	//f.gcount() - number of bytes
	return ofxKuHashCrc32(&buffer[0], len);
}

//--------------------------------------------------------------
