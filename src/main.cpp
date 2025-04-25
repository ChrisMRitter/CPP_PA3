// main.cpp
#include "enemyManager.h"
#include "player.h"
#include "textManager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

// Forward-declaration of the start screen
void startScreen(sf::RenderWindow& window, const sf::Font& font);

int main() {
    const float VIEW_WIDTH  = 800.f;
    const float VIEW_HEIGHT = 600.f;

    // Create window
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Space Shooter");
    window.setFramerateLimit(60);

    // Load full spritesheet
    sf::Texture sheet;
    if (!sheet.loadFromFile("./Assets/Sprites/spaceship_spritesheet.png")) {
        std::cerr << "Failed to load spritesheet\n";
        return -1;
    }
    sf::Image sheetImg = sheet.copyToImage();
    sf::Texture chaserTex, turretTex;
    chaserTex.loadFromImage(sheetImg, sf::IntRect(0*64, 0*64, 64, 64));
    turretTex.loadFromImage(sheetImg, sf::IntRect(4*64, 0*64, 64, 64));
    auto chaserPtr = std::make_shared<sf::Texture>(chaserTex);
    auto turretPtr = std::make_shared<sf::Texture>(turretTex);

    // Player setup
    Player player("./Assets/Sprites/hero spaceship.png", 300.f);

    // Load font for HUD and start screen
    sf::Font font;
    if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    // HUD manager
    textManager textMgr;
    if (!textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) {
        std::cerr << "Font failed to load in textMgr\n";
    }

    // Show the “Press Enter to Start” screen
    startScreen(window, font);

    // Enemy managers (unchanged)
    EnemyManager chasers;
    chasers.configure(chaserPtr, 12, 2, 1.5f, 120.f, 1.0f);

    EnemyManager turrets;
    turrets.configure(turretPtr, 6, 10, 5.0f, 30.f, 5.0f);

    // Background
    sf::Texture bgTex;
    if (!bgTex.loadFromFile("./Assets/Sprites/Planets and Space.png")) {
        std::cerr << "Failed to load background\n";
    }
    sf::Sprite background(bgTex);
    {
        auto ws = window.getSize();
        auto ts = bgTex.getSize();
        background.setScale(float(ws.x)/ts.x, float(ws.y)/ts.y);
    }

    // Main loop
    sf::Clock clock;
    while (window.isOpen()) {
        // 1) Poll events
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed)
                window.close();
        }

        // 2) Update delta time
        float dt = clock.restart().asSeconds();

        // 3) Update game logic
        player.update(dt);
        sf::Vector2f ppos = player.getPosition();
        chasers.update(dt, ppos);
        turrets.update(dt, ppos);

        // 4) Update HUD
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize   = window.getView().getSize();
        textMgr.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textMgr.updateScoreDisplay(viewCenter, viewSize);

        // 5) Draw everything
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

// ------------------------------------------------------------------------
// Press-Enter-To-Start screen
// ------------------------------------------------------------------------
void startScreen(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text prompt("Press Enter to Start", font, 50);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(
        window.getSize().x/2.f - prompt.getGlobalBounds().width/2.f,
        window.getSize().y/2.f - prompt.getGlobalBounds().height/2.f
    );

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();
            else if (ev.type == sf::Event::KeyPressed &&
                     ev.key.code == sf::Keyboard::Enter)
                return;
        }
        window.clear();
        window.draw(prompt);
        window.display();
    }
}

