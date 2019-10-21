#pragma once

//Encoding and decoding base64
//Code is based on https://github.com/ReneNyffenegger/cpp-base64 by Rene Nyffenegger

#include <string>

struct ofxKuUtilsBase64 {
	static std::string encode(unsigned char const*, unsigned int len);
	static std::string decode(std::string const& s);

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
