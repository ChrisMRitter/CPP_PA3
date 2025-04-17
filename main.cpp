// This will be the main game loop, and will handle all the repeating logic

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <player.cpp>
#include <player.h>

// Setting basic view size.
const float VIEW_WIDTH = 800.f;
const float VIEW_HEIGHT = 600.f;

// Placeholder functions
void handleEvents(sf::RenderWindow& window);
void startScreen(sf::RenderWindow& window, sf::Font font);

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "SFML window");

    // Create the player
    Player player("./Assets/Sprites/hero spaceship.png", 100);
//TODO: Chris, link the code above (and other Player code), make sure that it grabs a character model, maybe change it to grab from graphics folder


    // Load the font
    sf::Font font;
    if (!font.loadFromFile("./Assets/Fonts/Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) // <- move font to Graphics too if you want
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Clock for delta time
    sf::Clock clock;

    // Run the start screen function
    startScreen(window, font);

    // Load background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./Assets/Sprites/Planets and Space.png"))
    {
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    // Primary game loop
    while (window.isOpen())
    {
        // Handle events
        handleEvents(window);

        // Update player and mouse-world interaction
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        player.update(mouseWorldPos);

        // Clear screen
        window.clear();

        // Draw background and player
        window.draw(background);
        player.draw(window);

        // Display everything
        window.display();
    }

    return 0;
}

// Function to manage the start screen: gives instructions and waits for player to press enter
void startScreen(sf::RenderWindow& window, sf::Font font)
{
    sf::Text startText("Press Enter to Start\nPress Space to Fire", font, 50);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(window.getSize().x / 2.f - startText.getGlobalBounds().width / 2.f,
                           window.getSize().y / 2.f - startText.getGlobalBounds().height / 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                return; // exit start screen
        }
        window.clear();
        window.draw(startText);
        window.display();
    }
}

// Function to handle events with the render window
void handleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized)
        {
            float windowAspect = static_cast<float>(event.size.width) / event.size.height;
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