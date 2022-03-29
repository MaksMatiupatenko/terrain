#pragma once
#include <SFML/Graphics.hpp>

float length(const sf::Vector2f& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}
float length(const sf::Vector3f& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

sf::Vector3f normalize(const sf::Vector3f& vec) {
    return vec / length(vec);
}
sf::Vector2f normalize(const sf::Vector2f& vec) {
    return vec / length(vec);
}

sf::Vector3f setLength(const sf::Vector3f& vec, float length) {
    return normalize(vec) * length;
}

sf::Vector2f getAngles(const sf::Vector3f& vec) {
    return { atan2(vec.y, vec.x), atan2(vec.z, length(sf::Vector2f(vec.x, vec.y))) };
}

sf::Vector3f getRight(const sf::Vector3f& vec) {
    return setLength({ vec.y, -vec.x, 0 }, length(vec));
}

sf::Vector3f getUp(const sf::Vector3f& vec) {
    sf::Vector2f XY = normalize(sf::Vector2f(vec.x, vec.y)) * -vec.z;
    return { XY.x, XY.y, length(sf::Vector2f(vec.x, vec.y)) };
}

float dot(const sf::Vector3f& vec1, const sf::Vector3f& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

sf::Vector3f cross(const sf::Vector3f& vec1, const sf::Vector3f& vec2) {
    return sf::Vector3f(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}

sf::Vector3f toVecCoordSystem(const sf::Vector3f& vec, const sf::Vector3f& system) {
    return { dot(normalize(system), vec), dot(getRight(normalize(system)), vec), dot(getUp(normalize(system)), vec) };
}

sf::Vector2f operator*(const sf::Vector2f& vec1, const sf::Vector2f& vec2) {
    return { vec1.x * vec2.x, vec1.y * vec2.y };
}

sf::Vector3f buildVec(const sf::Vector2f& angles) {
    sf::Vector2f XY = sf::Vector2f(cos(angles.x), sin(angles.x)) * cos(angles.y);
    return { XY.x, XY.y, sin(angles.y) };
}

template <class T>
bool operator<(const sf::Vector2 <T>& a, const sf::Vector2 <T>& b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
}
