#pragma once 

//Statistics

#include "ofMain.h"

//mean value of the vector
template <typename T>
double ofxKuMean(const vector<T> &input) {
	if (input.empty()) return 0;
	double Sum = 0;
	for (auto &v : input) {
		Sum += v;
	}
	return Sum / input.size();
}

//mean index of the histogram
template <typename T>
double ofxKuMathMeanIndex(const vector<T> &input) {
	int n = input.size();
	if (n==0) return 0;
	double M = 0;
	double sum = 0;
	for (int i=0; i<n; i++) {
		M += i * input[i];
		sum += input[i];
	}
	if (sum>0) M/=sum;
	return M;
}

//median index of the histogram
template <typename T>
double ofxKuMathMedianIndex(const vector<T> &input) {
	int n = input.size();
	if (n==0) return 0;
	double sum = 0;
	for (int i=0; i<n; i++) {
		sum += input[i];
	}

	double q = 0;
	for (int i=0; i<n; i++) {
		if (q + input[i] > sum/2) return i;
		q += input[i];
	}
	return n-1;
}

//argmax, returns first index where maximal value is achieved
template <typename T>
int ofxKuArgMax(const vector<T> &input) {
	if (input.empty()) return 0;
	int j = 0;
	T V = input[0];
	for (int i = 1; i < input.size(); i++) {
		if (input[i] > V) {
			j = i;
			V = input[i];
		}
	}
	return j;
}


