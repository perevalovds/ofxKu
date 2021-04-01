#include "ofxKuRandom.h"

#include <random>

//--------------------------------------------------------------
//https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
std::random_device randomi_rd_;  //Will be used to obtain a seed for the random number engine

int ofxKuRandomInt(int a, int b_included) {
    if (a == b_included) return a;
    if (a > b_included) swap(a, b_included);
    std::mt19937 gen(randomi_rd_()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(a, b_included);
    return distrib(gen);
}

//--------------------------------------------------------------
//float xrandomf(float a, float b) {
//    std::uniform_real_distribution<> dist(a, b);
//    return dist(*QRandomGenerator::global());
//}

//--------------------------------------------------------------
