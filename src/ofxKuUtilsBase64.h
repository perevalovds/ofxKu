#pragma once

//Encoding and decoding base64
//Code is based on https://github.com/ReneNyffenegger/cpp-base64 by Rene Nyffenegger

#include "ofMain.h"

struct ofxKuUtilsBase64 {
	static std::string encode(unsigned char const*, unsigned int len);
	static std::string decode(std::string const& s);

	//use it in cases when sending via networking when required
	//wikipedia: url_safe '+/' -> '-_'.
	static std::string to_url_safe(std::string const& s);
	static std::string from_url_safe(std::string const& s);

	static std::string base64_chars() {
		return
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

	}



	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
};
