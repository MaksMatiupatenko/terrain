#pragma once
#include <SFML/Graphics.hpp>
#include "Vector.h"
#include "Base.h"

class Camera {
public:
    float moveSpeed = 1.0, rotationSpeed = 0.005;

    float FOV = PI / 2;

    sf::Vector3f pos;
    sf::Vector2f rot;
    sf::Vector3f forward, right, up;

    Camera() {
        sf::Mouse::setPosition(sf::Vector2i(1000, 500));
    }

    void update(float time) {
        sf::Vector3f shift;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            shift.x += moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            shift.x -= moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            shift.y += moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            shift.y -= moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            shift.z += moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            shift.z -= moveSpeed * time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            shift *= 5.0f;

        sf::Vector2i mouseShift = sf::Mouse::getPosition() - sf::Vector2i(1000, 500);
        sf::Mouse::setPosition(sf::Vector2i(1000, 500));
        rot += sf::Vector2f(-mouseShift.x, -mouseShift.y) * rotationSpeed;
        rot.y = std::min(PI / 2, std::max(-PI / 2, rot.y));

        forward = buildVec(rot);
        right = buildVec(sf::Vector2f(rot.x - PI / 2, 0));
        up = buildVec(sf::Vector2f(rot.x, rot.y + PI / 2));

        pos += forward * shift.x + right * shift.y + up * shift.z;
    }
};