// This will be the main game loop, and will handle all the repeating logic

#include "player.h" // Correct way to include your Player class
#include "textManager.h" // Include the textManager class
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Setting basic view size.
const float VIEW_WIDTH = 800.f;
const float VIEW_HEIGHT = 600.f;

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

    // Clock for delta time
    sf::Clock clock;

    // Music Setup
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("./Assets/Sound/ambient-soundscapes-004-space-atmosphere-303243.mp3")) {
        std::cerr << "Failed to load background music!" << std::endl;
    }
    // Loop the music and play it
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    
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

    // Primary game loop
    while (window.isOpen()) {
        // Delta time
        float dt = clock.restart().asSeconds();

        // Check the player's health
        checkPlayerHealth(player, window, font);

        // Handle events
        handleEvents(window);

        // Update player
        player.update(dt);

        // Update player health
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        textManager.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textManager.updateScoreDisplay(viewCenter, viewSize);

        // Clear screen
        window.clear();

        // Draw background, player, and text
        window.draw(background);
        player.draw(window);
        textManager.draw(window);

        // Display everything
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
