#pragma once
#include <SFML/Graphics.hpp>
#include "3Dstuff.h"
#include "Vector.h"


sf::Vector3f light = normalize(sf::Vector3f(-0.55, 0.45, -0.75));

static sf::Color getColor(float height) {
    if (height > 9) return sf::Color(255, 255, 255);
    if (height > 5) return sf::Color(120, 120, 120);
    if (height > 0.5) return sf::Color(20, 141, 54);
    if (height > 0.25) return sf::Color(140, 141, 46);
    if (height > 0) return sf::Color(150, 151, 14);
    return sf::Color(0, 162, 233);
}
static sf::Color getColor(const Triangle& p) {
    sf::Color col = getColor(std::max({ p.pos[0].z, p.pos[1].z, p.pos[2].z }));
    sf::Color rndCol(rnd(256), rnd(256), rnd(256));

    const float mul = 30;
    col.r = (col.r * mul + rndCol.r) / (mul + 1);
    col.g = (col.g * mul + rndCol.g) / (mul + 1);
    col.b = (col.b * mul + rndCol.b) / (mul + 1);

    return col;
}

static float getLightness(const Triangle& p) {
    float lightness = dot(p.norm(), -light);
    lightness = std::max(lightness, 0.0f);
    lightness *= 0.8;
    lightness += 0.2;
    return lightness;
}