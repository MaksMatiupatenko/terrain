#pragma once
#include <vector>
#include "base.h"

std::vector <std::vector <float>> genNoise(int szx, int szy, int act) {
    std::vector <std::vector <float>> height(szy, std::vector <float>(szx, 0));
        
    std::vector <std::vector <float>> noise(szy / (1 << act) + 1, std::vector <float>(szx / (1 << act) + 1, 0));
    for (auto& x : noise) {
        for (auto& y : x) {
            y = rnd01();
        }
    }

    for (int j = 0; j * (1 << act) < szy; ++j) {
        for (int k = 0; k * (1 << act) < szx; ++k) {
            for (int i1 = j * (1 << act); i1 < (j + 1) * (1 << act); ++i1) {
                for (int j1 = k * (1 << act); j1 < (k + 1) * (1 << act); ++j1) {
                    height[i1][j1] += mix(noise[j][k], noise[j + 1][k], noise[j][k + 1], noise[j + 1][k + 1],
                        ((float)j1 - k * (1 << act)) / (1 << act), ((float)i1 - j * (1 << act)) / (1 << act));
                }
            }
        }
    }

    return height;
}
std::vector <std::vector <float>> genNoise(int szx, int szy, int actl, int actr) {
    int lcnt = 0;
    std::vector <std::vector <float>> height(szy, std::vector <float>(szx, 0));
    for (int i = actl; i < actr; ++i) {
        float w = i * i + 1;

        lcnt += w;
        std::vector <std::vector <float>> noise = genNoise(szx, szy, i);
        for (int j = 0; j < szy; ++j) {
            for (int k = 0; k < szx; ++k) {
                height[j][k] += noise[j][k] * w;
            }
        }
    }
    for (auto& x : height) {
        for (auto& y : x) {
            y /= lcnt;
        }
    }

    return height;
}


std::vector <std::vector <float>> genNoise(int size, int act, sf::Vector2i pos) {
    std::vector <std::vector <float>> height(size + 1, std::vector <float>(size + 1, 0));

    std::vector <std::vector <float>> noise(size / (1 << act) + 2, std::vector <float>(size / (1 << act) + 2, 0));
    for (int i = 0; i < noise.size(); ++i) {
        for (int j = 0; j < noise[0].size(); ++j) {
            rnd_ = std::mt19937(initSeed + (act + 17857) * ((((dv(pos.y * size, 1 << act) + j) << 16) + dv(pos.x * size, 1 << act) + i)));
            noise[i][j] = rnd01();
        }
    }

    for (int j = 0; j < noise.size() - 1; ++j) {
        for (int k = 0; k < noise.size() - 1; ++k) {
            for (int i1 = j * (1 << act); i1 < (j + 1) * (1 << act) && i1 < height.size(); ++i1) {
                for (int j1 = k * (1 << act); j1 < (k + 1) * (1 << act) && j1 < height[0].size(); ++j1) {
                    height[i1][j1] += mix(noise[j][k], noise[j + 1][k], noise[j][k + 1], noise[j + 1][k + 1],
                        (mod(pos.y * size, 1 << act) + (float)j1 - k * (1 << act)) / (1 << act), (mod(pos.x * size, 1 << act) + (float)i1 - j * (1 << act)) / (1 << act));
                }
            }
        }
    }

    return height;
}
std::vector <std::vector <float>> genNoise(int size, int actl, int actr, sf::Vector2i pos) {
    int lcnt = 0;
    std::vector <std::vector <float>> height(size + 1, std::vector <float>(size + 1, 0));
    for (int i = actl; i < actr; ++i) {
        float w = i * i + 1;

        lcnt += w;
        std::vector <std::vector <float>> noise = genNoise(size, i, pos);
        for (int j = 0; j < size + 1; ++j) {
            for (int k = 0; k < size + 1; ++k) {
                height[j][k] += noise[j][k] * w;
            }
        }
    }
    for (auto& x : height) {
        for (auto& y : x) {
            y /= lcnt;
        }
    }

    return height;
}
