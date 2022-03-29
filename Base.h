#pragma once
#include <chrono>
#include <random>

const float PI = 3.1415926535;
uint32_t initSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 rnd_(initSeed);
uint32_t rnd() {
    return rnd_();
}
float rnd01() {
    return (float)rnd_() / std::mt19937::max();
}
uint32_t rnd(uint32_t m) {
    return rnd_() % m;
}

template <class T>
T mix(const T& a, const T& b, float k) {
    return a * (1 - k) + b * k;
}

template <class T>
T mix(const T& q11, const T& q12, const T& q21, const T& q22, float x, float y) {
    return q11 * (1 - x) * (1 - y) + q12 * (1 - x) * y + q21 * x * (1 - y) + q22 * x * y;
}

int mod(int a, int b) {
    return (a % b + b) % b;
}
int dv(int a, int b) {
    if (a < 0) return (a - b + 1) / b;
    return a / b;
}
