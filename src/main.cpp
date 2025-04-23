// This will be the main game loop, and will handle all the repeating logic

#include "player.h" // Correct way to include your Player class
#include "textManager.h" // Include the textManager class
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "laser.h"
#include <SFML/Audio.hpp>
#include "gameconstants.h"


// Placeholder functions
void handleEvents(sf::RenderWindow& window);
void startScreen(sf::RenderWindow& window, sf::Font font);
void checkPlayerHealth(Player& player, sf::RenderWindow& window, sf::Font font);

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT),
        "SFML window");

    // Create the player — spaceship that moves left and right only
    Player player("./Assets/Sprites/hero spaceship.png", 300.f);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile(
        "./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) // <- Adjusted path
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    textManager textManager;
    textManager.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

    std::vector<Laser> lasers;

    // Clock for delta time
    sf::Clock clock;

    // Run the start screen function
    startScreen(window, font);


    // Load background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(
        "./Assets/Sprites/Planets and Space.png")) {
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    // Scale the background to fit the window
    sf::Vector2u textureSize = backgroundTexture.getSize(); // Original image size
    sf::Vector2u windowSize = window.getSize();             // Current window size

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    background.setScale(scaleX, scaleY);

    //TODO: Music and sound, level manager, etc.

    // Configure music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Eschatos.mp3"))
    {
        std::cerr << "Failed to load background music!" << std::endl;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // Primary game loop
    while (window.isOpen()) {
        // Delta time: framerate independent physics
        float dt = clock.restart().asSeconds();

        // Check the player's health
        checkPlayerHealth(player, window, font);

        // Handle events
        handleEvents(window);


        // Get mouse position for aiming
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Update player
        player.update(dt, lasers, mouseWorldPos);

        // Update level (enemies, obstacles, etc.) - may add later
        //levelManager.update(dt, player, lasers);


        //update UI elements
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();

        textManager.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textManager.updateScoreDisplay(viewCenter, viewSize);

        // Update player health

        textManager.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textManager.updateScoreDisplay(viewCenter, viewSize);

        // Update camera to follow player
        sf::View view = window.getView();
        view.setCenter(player.getPosition());
        window.setView(view);

        // Clear screen
        window.clear();

        // Draw background, player, and text
        sf::View fixedView = window.getDefaultView(); //need to untether the background from the gameplay area, to keep background constant
        window.setView(fixedView);
        window.draw(background);
        window.setView(view); //reset to player view for game elements
        player.draw(window);
        textManager.draw(window);

        //Display and update lasers

        for (auto laser = lasers.begin(); laser != lasers.end();) {
            laser->update(dt);
            laser->draw(window);
            if (laser->isOutOfBounds())
                laser = lasers.erase(laser);
            else
                ++laser;
        }
                

        // Display everything else
        window.display();


    }

    return 0;
}

// Function to manage the start screen
void startScreen(sf::RenderWindow& window, sf::Font font) {
    sf::Text startText("Press Enter to Start", font, 50);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(
        window.getSize().x / 2.f - startText.getGlobalBounds().width / 2.f,
        window.getSize().y / 2.f - startText.getGlobalBounds().height / 2.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter)
                return;
        }

        window.clear();
        window.draw(startText);
        window.display();
    }
}

// Function to check the player's health, and trigger the end game.
void checkPlayerHealth(Player& player, sf::RenderWindow& window, sf::Font font) {
    if (player.getHealth() <= 0) {
        sf::Text gameOverText("Game Over \n Press esc to quit", font, 50);
        gameOverText.setFillColor(sf::Color::Red);
        // Set the position to center on the screen
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        gameOverText.setPosition(viewCenter.x - gameOverText.getGlobalBounds().width / 2.f, viewCenter.y - gameOverText.getGlobalBounds().height / 2.f);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            window.clear();
            window.draw(gameOverText);
            window.display();
        }
    }
}

// Function to handle events
void handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized) {
            float windowAspect =
                static_cast<float>(event.size.width) / event.size.height;
            float viewAspect = VIEW_WIDTH / VIEW_HEIGHT;
            sf::View view = window.getView();

            if (windowAspect > viewAspect)
                view.setSize(VIEW_HEIGHT * windowAspect, VIEW_HEIGHT);
            else
                view.setSize(VIEW_WIDTH, VIEW_WIDTH / windowAspect);
            window.setView(view);
        }
    }
}
