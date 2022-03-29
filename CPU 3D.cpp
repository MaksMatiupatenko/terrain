#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "3Dstuff.h"
#include "Base.h"
#include <chrono>
#include "Noise.h"
#include "Colors.h"
#include "Terrain.h"

void saveScreenshot(const sf::RenderWindow& window) {
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    sf::Image screenshot = texture.copyToImage();
    screenshot.saveToFile("screenshot/" + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + ".png");
}

int main() {
    sf::ContextSettings settings;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "window", sf::Style::Fullscreen, settings);
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(30);

    Camera camera;
    camera.pos = { 0, 0, 5 };
    camera.rot = { 0, 0 };
    camera.moveSpeed = 20;

    Terrain vec(16);


    sf::Clock clock;
    while (window.isOpen()) {
        float time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F12) {
                    saveScreenshot(window);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    initSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                    vec.reset();
                }
            }
        }
        if (!window.hasFocus()) continue;

        camera.update(time);

        window.clear();
        vec.draw(window, camera);
        window.display();
    }
}