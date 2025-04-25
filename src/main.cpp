// main.cpp
#include "enemyManager.h"
#include "player.h"
#include "textManager.h"
#include "laser.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

// Constants for window size
static const float VIEW_WIDTH  = 800.f;
static const float VIEW_HEIGHT = 600.f;

// Forward declarations
void startScreen(sf::RenderWindow& window, const sf::Font& font);
void checkPlayerHealth(Player& player, sf::RenderWindow& window, const sf::Font& font);
void handleEvents(sf::RenderWindow& window);

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Space Shooter");
    window.setFramerateLimit(60);

    // Load sprite sheet and crop two textures
    sf::Texture sheet;
    if (!sheet.loadFromFile("./Assets/Sprites/spaceship_spritesheet.png")) {
        std::cerr << "Failed to load spritesheet" << std::endl;
        return -1;
    }
    sf::Image sheetImg = sheet.copyToImage();
    sf::Texture chaserTex, turretTex;
    // Column 1, row 2 (zero-based col=0,row=1)
    chaserTex.loadFromImage(sheetImg, sf::IntRect(0 * 64, 0 * 64, 64, 64));
    // Column 3, row 2 (zero-based col=2,row=1)
    turretTex.loadFromImage(sheetImg, sf::IntRect(0 * 64, 4 * 64, 64, 64));
    auto chaserPtr = std::make_shared<sf::Texture>(chaserTex);
    auto turretPtr = std::make_shared<sf::Texture>(turretTex);

    // Player and laser container
    Player player("./Assets/Sprites/hero spaceship.png", 300.f);
    std::vector<Laser> lasers;

    // Load font for HUD and start screen
    sf::Font font;
    if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }
    textManager textMgr;
    textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

    // Show start screen
    startScreen(window, font);

    // Configure enemy managers with lower spawn rate
    EnemyManager chasers;
    chasers.configure(chaserPtr, 12, 2, 2.5f, 120.f, 1.0f);
    EnemyManager turrets;
    turrets.configure(turretPtr, 6, 10, 6.0f, 30.f, 5.0f);

    // Load and scale background
    sf::Texture bgTex;
    if (!bgTex.loadFromFile("./Assets/Sprites/Planets and Space.png")) {
        std::cerr << "Failed to load background" << std::endl;
    }
    sf::Sprite background(bgTex);
    {
        auto ws = window.getSize();
        auto ts = bgTex.getSize();
        background.setScale(ws.x / static_cast<float>(ts.x),
                             ws.y / static_cast<float>(ts.y));
    }

    // Play background music
    sf::Music bgMusic;
    if (bgMusic.openFromFile("./Assets/Sound/Eschatos.mp3")) {
        bgMusic.setLoop(true);
        bgMusic.play();
    }

    // Main loop
    sf::Clock clock;
    while (window.isOpen()) {
        // Handle events and check health
        handleEvents(window);
        checkPlayerHealth(player, window, font);

        // Delta time
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // Update player (including firing)
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        player.update(dt, lasers, mousePos);

        // Update lasers
        for (auto& laser : lasers) {
            laser.update(dt);
        }

        // Update enemies (spawn from top, move)
        sf::Vector2f ppos = player.getPosition();
        chasers.update(dt, ppos);
        turrets.update(dt, ppos);

        // Laser-enemy collisions
        chasers.handleLaserCollisions(lasers);
        turrets.handleLaserCollisions(lasers);

        // Enemy-player collisions (damage values)
        chasers.handlePlayerCollisions(player, 1);
        turrets.handlePlayerCollisions(player, 5);

        // Update HUD
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize   = window.getView().getSize();
        textMgr.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textMgr.updateScoreDisplay(viewCenter, viewSize);

        // Draw everything
        window.clear();
        window.draw(background);
        chasers.draw(window);
        turrets.draw(window);
        player.draw(window);
        for (auto& laser : lasers) {
            laser.draw(window);
        }
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
            else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter)
                return;
        }
        window.clear();
        window.draw(prompt);
        window.display();
    }
}

// ------------------------------------------------------------------------
// Check player health and show Game Over
// ------------------------------------------------------------------------
void checkPlayerHealth(Player& player, sf::RenderWindow& window, const sf::Font& font) {
    if (player.getHealth() <= 0) {
        sf::Text gameOver("Game Over\nPress Esc to Quit", font, 50);
        gameOver.setFillColor(sf::Color::Red);
        gameOver.setPosition(
            window.getSize().x/2.f - gameOver.getGlobalBounds().width/2.f,
            window.getSize().y/2.f - gameOver.getGlobalBounds().height/2.f
        );
        while (window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed)
                    window.close();
                else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            window.clear();
            window.draw(gameOver);
            window.display();
        }
    }
}

// ------------------------------------------------------------------------
// Basic event handling
// ------------------------------------------------------------------------
void handleEvents(sf::RenderWindow& window) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed)
            window.close();
        else if (ev.type == sf::Event::Resized) {
            float winA = static_cast<float>(ev.size.width)/ev.size.height;
            float viewA = VIEW_WIDTH/VIEW_HEIGHT;
            sf::View v = window.getView();
            if (winA > viewA)
                v.setSize(VIEW_HEIGHT * winA, VIEW_HEIGHT);
            else
                v.setSize(VIEW_WIDTH, VIEW_WIDTH / winA);
            window.setView(v);
        }
    }
}

