#include "enemyManager.h"
#include "player.h"
#include "textManager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Space Shooter");
    window.setFramerateLimit(60);

    // Load full spritesheet
    sf::Texture sheet;
    if (!sheet.loadFromFile("./Assets/Sprites/spaceship_spritesheet.png")) {
        std::cerr << "Failed to load spritesheet" << std::endl;
        return -1;
    }

    // Crop two 64×64 regions
    sf::Image sheetImg = sheet.copyToImage();
    sf::Texture chaserTex, turretTex;
    chaserTex.loadFromImage(sheetImg, sf::IntRect(0 * 64, 0 * 64, 64, 64));
    turretTex.loadFromImage(sheetImg, sf::IntRect(4 * 64, 0 * 64, 64, 64));

    auto chaserPtr = std::make_shared<sf::Texture>(chaserTex);
    auto turretPtr = std::make_shared<sf::Texture>(turretTex);

    // Player setup
    Player player("./Assets/Sprites/hero spaceship.png", 300.f);

    // HUD
    textManager textMgr;
    if (!textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf"))
        std::cerr << "Font failed to load" << std::endl;

    // Enemy managers: each with its own texture, scale, and dynamic attributes
    EnemyManager chasers;
    chasers.configure(chaserPtr,  12,   2, 1.5f, 120.f, 1.0f);

    EnemyManager turrets;
    turrets.configure(turretPtr,   6,  10, 5.0f,   30.f, 5.0f);

    // Background (optional)
    sf::Texture bgTex;
    if (!bgTex.loadFromFile("./Assets/Sprites/Planets and Space.png"))
        std::cerr << "Failed to load background" << std::endl;
    sf::Sprite background(bgTex);
    auto ws = window.getSize();
    auto ts = bgTex.getSize();
    background.setScale(float(ws.x)/ts.x, float(ws.y)/ts.y);

    // Main loop
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) window.close();
        }
        float dt = clock.restart().asSeconds();

        // Update
        player.update(dt);
        sf::Vector2f ppos = player.getPosition();
        chasers.update(dt, ppos);
        turrets.update(dt, ppos);

        // Draw
        window.clear();
        window.draw(background);
        chasers.draw(window);
        turrets.draw(window);
        player.draw(window);
        textMgr.draw(window);
        window.display();
    }
    return 0;
}

