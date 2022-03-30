#pragma once
#include "Noise.h"
#include "3Dstuff.h"
#include "Colors.h"
#include "Vector.h"
#include <set>
#include <algorithm>

class Chunk {
private:

    sf::Vector2i pos;


    std::vector <Triangle> generate(int pcnt, int l, int r, int detail, const std::vector <bool>& end) {
        l = std::max(l, detail);

        std::vector <std::vector <float>> height = genNoise(pcnt, l, r, pos, detail, end);
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
        for (int i = 0; i < (pcnt >> detail); ++i) {
            for (int j = 0; j < (pcnt >> detail); ++j) {
                Triangle p;
                p.pos[0] = { (float)(i << detail) + pos.x * size, (float)(j << detail) + pos.y * size, height[i][j] };
                p.pos[1] = { (float)((i + 1) << detail) + pos.x * size, (float)(j << detail) + pos.y * size, height[i + 1][j] };
                p.pos[2] = { (float)(i << detail) + pos.x * size, (float)((j + 1) << detail) + pos.y * size, height[i][j + 1] };
                p.color = getColor(p);

                float lightness = getLightness(p);
                p.color.r *= lightness;
                p.color.g *= lightness;
                p.color.b *= lightness;

                vec.push_back(p);


                p.pos[0] = { (float)((i + 1) << detail) + pos.x * size, (float)((j + 1) << detail) + pos.y * size, height[i + 1][j + 1] };
                p.pos[1] = { (float)((i + 1) << detail) + pos.x * size, (float)(j << detail) + pos.y * size, height[i + 1][j] };
                p.pos[2] = { (float)(i << detail) + pos.x * size, (float)((j + 1) << detail) + pos.y * size, height[i][j + 1] };
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
    std::vector <bool> end;
    std::vector <Triangle> data;

    Chunk(sf::Vector2i pos, int detail, const std::vector <bool>& ed) :pos(pos) {
        end = ed;
        data = generate(size,0, 8, detail, ed);
    }
    Chunk() = default;

    float dist(const Camera& camera) const {
        return length((sf::Vector2f)pos - sf::Vector2f(camera.pos.x, camera.pos.y));
    }
};

class Terrain {
private:
    std::vector <std::map <std::pair <int, int>, Chunk>> data;
    std::vector <int> size;

    int getDetail(const std::pair <int, int>& p) {
        return std::lower_bound(size.begin(), size.end(), std::max(abs(p.first), abs(p.second))) - size.begin();
    }

    std::vector <bool> getChunkEnd(const std::pair <int, int>& p, int detail) {
        auto [i, j] = p;
        return { getDetail({i, j - 1}) > detail, getDetail({i - 1, j}) > detail, getDetail({i, j + 1}) > detail, getDetail({i + 1, j}) > detail };
    }

public:
    Terrain(const std::vector <int>& size) :size(size) {
        data.resize(size.size());
    }

    void draw(sf::RenderWindow& window, const Camera& camera) {
        std::map <sf::Vector2i, Chunk> newData;
        StuffArray <Triangle> vec;

        int curx = floor(camera.pos.x / Chunk::size);
        int cury = floor(camera.pos.y / Chunk::size);

        for (int i = 0; i < data.size(); ++i) {
            std::vector <std::pair <int, int>> toDelete;
            for (const auto& [k, v] : data[i]) {
                if (getDetail({ k.first - curx, k.second - cury }) != i || v.end != getChunkEnd({ k.first - curx, k.second - cury }, i)) {
                    toDelete.push_back(k);
                }
            }
            for (const auto& k : toDelete) {
                data[i].erase(k);
            }
        }

        for (int i = curx - size.back(); i <= curx + size.back(); ++i) {
            for (int j = cury - size.back(); j <= cury + size.back(); ++j) {
                int detail = getDetail({ i - curx, j - cury });
                if (!data[detail].contains({ i, j })) {
                    std::vector <bool> end = getChunkEnd({ i - curx, j - cury }, detail);
                    //bool end[4] = { false, false, false, false };
                    data[detail][{i, j}] = Chunk(sf::Vector2i(i, j), detail, end);
                }

                for (auto& p : data[detail][{i, j}].data) {
                    vec.push(p);
                }
            }
        }

        vec.draw(window, camera);
    }

    void reset() {
        for (auto& mp : data) mp.clear();
    }
};
