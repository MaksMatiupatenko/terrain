#pragma once
#include "Noise.h"
#include "3Dstuff.h"
#include "Colors.h"
#include "Vector.h"
#include <set>

class Chunk {
private:

    sf::Vector2i pos;


    std::vector <Triangle> generate(int pcnt, int l, int r) {
        std::vector <std::vector <float>> height = genNoise(pcnt, l, r, pos);
        for (auto& x : height) {
            for (auto& y : x) {
                y -= 0.40;
                y = std::max(0.0f, y);
                y *= 10;
                y = pow(y, 2);
            }
        }

        sf::Vector3f light = normalize(sf::Vector3f(-0.55, 0.45, -0.75));

        std::vector <Triangle> vec;
        for (int i = 0; i < pcnt; ++i) {
            for (int j = 0; j < pcnt; ++j) {
                Triangle p;
                p.pos[0] = { (float)i + pos.x * size, (float)j + pos.y * size, height[i][j] };
                p.pos[1] = { (float)i + 1 + pos.x * size, (float)j + pos.y * size, height[i + 1][j] };
                p.pos[2] = { (float)i + pos.x * size, (float)j + 1 + pos.y * size, height[i][j + 1] };
                p.color = getColor(p);

                float lightness = getLightness(p);
                p.color.r *= lightness;
                p.color.g *= lightness;
                p.color.b *= lightness;

                vec.push_back(p);


                p.pos[0] = { (float)i + 1 + pos.x * size, (float)j + 1 + pos.y * size, height[i + 1][j + 1] };
                p.pos[1] = { (float)i + 1 + pos.x * size, (float)j + pos.y * size, height[i + 1][j] };
                p.pos[2] = { (float)i + pos.x * size, (float)j + 1 + pos.y * size, height[i][j + 1] };
                p.color = getColor(p);

                lightness = getLightness(p);
                p.color.r *= lightness;
                p.color.g *= lightness;
                p.color.b *= lightness;

                vec.push_back(p);
            }
        }

        return vec;
    }

public:
    static const int size = 16;
    std::vector <Triangle> data;

    Chunk(sf::Vector2i pos) :pos(pos) {
        data = generate(size, 0, 8);
    }
    Chunk() = default;

    float dist(const Camera& camera) const {
        return length((sf::Vector2f)pos - sf::Vector2f(camera.pos.x, camera.pos.y));
    }
};

class Terrain {
private:
    std::map <std::pair <int, int>, Chunk> data;
    int size;

public:
    Terrain(int size) :size(size) {}

    void draw(sf::RenderWindow& window, const Camera& camera) {
        std::map <sf::Vector2i, Chunk> newData;
        StuffArray <Triangle> vec;

        int fx = floor(camera.pos.x / Chunk::size) - size / 2;
        int cx = floor(camera.pos.x / Chunk::size) + size / 2;
        int fy = floor(camera.pos.y / Chunk::size) - size / 2;
        int cy = floor(camera.pos.y / Chunk::size) + size / 2;

        std::vector <std::pair <int, int>> toDelete;
        for (const auto& [k, v] : data) {
            if (!(fx <= k.first && k.first < cx && fy <= k.second && k.second < cy)) {
                toDelete.push_back(k);
            }
        }
        for (const auto& k : toDelete) {
            data.erase(k);
        }

        for (int i = fx; i < cx; ++i) {
            for (int j = fy; j < cy; ++j) {
                if (!data.contains({ i, j })) {
                    data[{i, j}] = Chunk(sf::Vector2i(i, j));
                }

                for (auto& p : data[{i, j}].data) {
                    vec.push(p);
                }
            }
        }

        vec.draw(window, camera);
    }

    void reset() {
        data.clear();
    }
};