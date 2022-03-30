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


std::vector <std::vector <float>> genNoise(int size, int act, sf::Vector2i pos, int detail) {
    std::vector <std::vector <float>> height((size >> detail) + 1, std::vector <float>((size >> detail) + 1, 0));

    std::vector <std::vector <float>> noise(size / (1 << act) + 2, std::vector <float>(size / (1 << act) + 2, 0));
    for (int i = 0; i < noise.size(); ++i) {
        for (int j = 0; j < noise[0].size(); ++j) {
            rnd_ = std::mt19937(initSeed + (act + 17857) * ((((dv(pos.y * size, 1 << act) + j) << 16) + dv(pos.x * size, 1 << act) + i)));
            noise[i][j] = rnd01();
        }
    }

    for (int j = 0; j < noise.size() - 1; ++j) {
        for (int k = 0; k < noise.size() - 1; ++k) {
            for (int i1 = (j * (1 << act)) >> detail; i1 < (((j + 1) * (1 << act)) >> detail) && i1 < height.size(); ++i1) {
                for (int j1 = (k * (1 << act)) >> detail; j1 < (((k + 1) * (1 << act)) >> detail) && j1 < height[0].size(); ++j1) {
                    height[i1][j1] += mix(noise[j][k], noise[j + 1][k], noise[j][k + 1], noise[j + 1][k + 1],
                        (mod(pos.y * (size >> detail), (1 << act) >> detail) + (float)j1 - k * ((1 << act) >> detail)) / ((1 << act) >> detail), (mod(pos.x * (size >> detail), (1 << act) >> detail) + (float)i1 - j * ((1 << act) >> detail)) / ((1 << act) >> detail));
                }
            }
        }
    }

    return height;
}
std::vector <std::vector <float>> genNoise(int size, int actl, int actr, sf::Vector2i pos, int detail, const std::vector <bool>& end) {
    std::vector <std::vector <float>> height((size >> detail) + 1, std::vector <float>((size >> detail) + 1, 0));
    std::vector <std::vector <float>> lcnt((size >> detail) + 1, std::vector <float>((size >> detail) + 1, 0));
    for (int i = actl; i < actr; ++i) {
        float w = i * i + 1;

        std::vector <std::vector <float>> noise = genNoise(size, i, pos, detail);

        for (auto& raw : lcnt) {
            for (auto& elem : raw) {
                elem += w;
            }
        }

        if (i == detail) {
            if (end[0]) {
                for (int i = 0; i < noise.size(); ++i) {
                    noise[i][0] = 0;
                    lcnt[i][0] -= w;
                }
            }
            if (end[1]) {
                for (int i = 0; i < noise.size(); ++i) {
                    noise[0][i] = 0;
                    lcnt[0][i] -= w;
                }
            }
            if (end[2]) {
                for (int i = 0; i < noise.size(); ++i) {
                    noise[i][noise.size() - 1] = 0;
                    lcnt[i][noise.size() - 1] -= w;
                }
            }
            if (end[3]) {
                for (int i = 0; i < noise.size(); ++i) {
                    noise[noise.size() - 1][i] = 0;
                    lcnt[noise.size() - 1][i] -= w;
                }
            }
        }
        else {
            if (end[0]) {
                for (int i = 1; i < noise.size() - 1; i += 2) {
                    noise[i][0] = (noise[i - 1][0] + noise[i + 1][0]) / 2;
                }
            }
            if (end[1]) {
                for (int i = 1; i < noise.size() - 1; i += 2) {
                    noise[0][i] = (noise[0][i - 1] + noise[0][i + 1]) / 2;
                }
            }
            if (end[2]) {
                for (int i = 1; i < noise.size() - 1; i += 2) {
                    noise[i][noise.size() - 1] = (noise[i - 1][noise.size() - 1] + noise[i + 1][noise.size() - 1]) / 2;
                }
            }
            if (end[3]) {
                for (int i = 1; i < noise.size() - 1; i += 2) {
                    noise[noise.size() - 1][i] = (noise[noise.size() - 1][i - 1] + noise[noise.size() - 1][i + 1]) / 2;
                }
            }
        }

        for (int j = 0; j < height.size(); ++j) {
            for (int k = 0; k < height.size(); ++k) {
                height[j][k] += noise[j][k] * w;
            }
        }
    }
    for (int i = 0; i < height.size(); ++i) {
        for (int j = 0; j < height.size(); ++j) {
            height[i][j] /= lcnt[i][j];
        }
    }

    return height;
}
