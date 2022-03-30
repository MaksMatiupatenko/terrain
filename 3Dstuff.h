#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Vector.h"
#include <algorithm>

sf::Vector2f project(sf::Vector3f pos, const sf::RenderWindow& window, const Camera& camera) {
    sf::Vector3f dir = normalize(pos - camera.pos);
    sf::Vector2f screenPos(dot(dir, camera.right) / std::max(dot(dir, camera.forward), 0.0001f), dot(dir, camera.up) / std::max(dot(dir, camera.forward), 0.0001f));
    screenPos *= PI / camera.FOV;
    screenPos.y *= -1;

    screenPos += sf::Vector2f(1, 1.0 * window.getSize().y / window.getSize().x);
    screenPos /= 2.0f;

    sf::Vector2f windowPos(screenPos.x * window.getSize().x, screenPos.y * window.getSize().x);

    return windowPos;
}

class Point {
public:
    sf::Vector3f pos;
    sf::Color color;
    
    static const sf::PrimitiveType primitiveType = sf::PrimitiveType::Points;

    Point() = default;
    Point(const sf::Vector3f& pos, const sf::Color& color) : pos(pos), color(color) {};

    void draw(sf::RenderWindow& window, const Camera& camera, sf::VertexArray& arr) const {
        if (dot(normalize(pos - camera.pos), camera.forward) > 0) {
            arr.append(sf::Vertex(project(pos, window, camera), color));
        }
    }

    float dist(const Camera& camera) const {
        return length(pos - camera.pos);
    }

    friend void swap(Point& a, Point& b) {
        std::swap(a.pos, b.pos);
        std::swap(a.color, b.color);
    }
};

class Line {
public:
    sf::Vector3f pos[2];
    sf::Color color;

    static const sf::PrimitiveType primitiveType = sf::PrimitiveType::Lines;

    Line() = default;
    Line(sf::Vector3f _pos[2], const sf::Color& color) : color(color) {
        for (int i = 0; i < 2; ++i) {
            pos[i] = _pos[i];
        }
    }

    void draw(sf::RenderWindow& window, const Camera& camera, sf::VertexArray& arr) {
        float d0 = dot(normalize(pos[0] - camera.pos), camera.forward);
        float d1 = dot(normalize(pos[1] - camera.pos), camera.forward);

        bool flag0 = (d0 > 0);
        bool flag1 = (d1 > 0);

        if (flag1) {
            std::swap(pos[0], pos[1]);
            std::swap(flag0, flag1);
            std::swap(d0, d1);
        }

        if (!flag0) return;

        if (flag1) {
            arr.append(sf::Vertex(project(pos[0], window, camera), color));
            arr.append(sf::Vertex(project(pos[1], window, camera), color));
            return;
        }

        d1 *= -1;
        arr.append(sf::Vertex(project(pos[0], window, camera), color));
        arr.append(sf::Vertex(project(mix(pos[1], pos[0], d1 / (d0 + d1)), window, camera), color));
    }

    float dist(const Camera& camera) const {
        return (length(pos[0] - camera.pos) + length(pos[1] - camera.pos)) / 2;
    }
};

class Triangle {
public:
    sf::Vector3f pos[3];
    sf::Color color;

    static const sf::PrimitiveType primitiveType = sf::PrimitiveType::Triangles;

    Triangle() = default;
    Triangle(sf::Vector3f _pos[3], const sf::Color& color) : color(color) {
        for (int i = 0; i < 3; ++i) {
            pos[i] = _pos[i];
        }
    }

    void draw(sf::RenderWindow& window, const Camera& camera, sf::VertexArray& arr) {
        float d0 = dot(normalize(pos[0] - camera.pos), camera.forward);
        float d1 = dot(normalize(pos[1] - camera.pos), camera.forward);
        float d2 = dot(normalize(pos[2] - camera.pos), camera.forward);

        bool flag0 = (d0 > 0);
        bool flag1 = (d1 > 0);
        bool flag2 = (d2 > 0);

        if (flag1) {
            std::swap(pos[0], pos[1]);
            std::swap(flag0, flag1);
            std::swap(d0, d1);
        }
        if (flag2) {
            std::swap(pos[1], pos[2]);
            std::swap(flag1, flag2);
            std::swap(d1, d2);
        }
        if (flag1) {
            std::swap(pos[0], pos[1]);
            std::swap(flag0, flag1);
            std::swap(d0, d1);
        }

        if (!flag0) {
            return;
        }
        if (!flag1) {
            d1 *= -1;
            d2 *= -1;
            arr.append(sf::Vertex(project(pos[0], window, camera), color));
            arr.append(sf::Vertex(project(mix(pos[1], pos[0], d1 / (d0 + d1)), window, camera), color));
            arr.append(sf::Vertex(project(mix(pos[2], pos[0], d2 / (d0 + d2)), window, camera), color));
            return;
        }
        if (!flag2) {
            d2 *= -1;
            arr.append(sf::Vertex(project(pos[0], window, camera), color));
            arr.append(sf::Vertex(project(pos[1], window, camera), color));
            arr.append(sf::Vertex(project(mix(pos[2], pos[1], d2 / (d1 + d2)), window, camera), color));

            arr.append(sf::Vertex(project(pos[1], window, camera), color));
            arr.append(sf::Vertex(project(mix(pos[2], pos[1], d2 / (d1 + d2)), window, camera), color));
            arr.append(sf::Vertex(project(mix(pos[2], pos[0], d2 / (d0 + d2)), window, camera), color));
            return;
        }

        arr.append(sf::Vertex(project(pos[0], window, camera), color));
        arr.append(sf::Vertex(project(pos[1], window, camera), color));
        arr.append(sf::Vertex(project(pos[2], window, camera), color));
    }

    float dist(const Camera& camera) const {
        return (length(pos[0] - camera.pos) + length(pos[1] - camera.pos) + length(pos[2] - camera.pos)) / 3;
    }

    sf::Vector3f norm() const {
        sf::Vector3f vec = normalize(cross(pos[1] - pos[0], pos[2] - pos[0]));
        if (vec.z < 0) vec *= -1.0f;
        return vec;
    }

    bool cmp(const Triangle& other, const Camera& camera) const {
        float d0 = dot(norm(), other.pos[0] - pos[0]);
        float d1 = dot(norm(), other.pos[1] - pos[0]);
        float d2 = dot(norm(), other.pos[2] - pos[0]);

        if (d0 * d1 >= 0 && d1 * d2 >= 0) {
            return dot(norm(), camera.pos - pos[0]) * d0 <= 0;
        }

        d0 = dot(other.norm(), pos[0] - other.pos[0]);
        d1 = dot(other.norm(), pos[1] - other.pos[0]);
        d2 = dot(other.norm(), pos[2] - other.pos[0]);

        if (d0 * d1 >= 0 && d1 * d2 >= 0) {
            return dot(norm(), camera.pos - other.pos[0]) * d0 >= 0;
        }

        return dist(camera) < other.dist(camera);
    }
};


template <class Stuff>
class StuffArray {
private:
    std::vector <Stuff> data;

public:
    void push(const Stuff& p) {
        data.push_back(p);
    }

    void draw(sf::RenderWindow& window, const Camera& camera) {
        stable_sort(data.rbegin(), data.rend(), [&camera](const Stuff& a, const Stuff& b) {
            return a.dist(camera) < b.dist(camera);
           });
        
        sf::VertexArray vec(Stuff::primitiveType);
        for (auto& p : data) {
            p.draw(window, camera, vec);
        }

        window.draw(vec);
    }
};
