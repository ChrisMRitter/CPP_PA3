// This will be the main game loop, and will handle all the repeating logic
// Sources for the sprites
//
// 
// Music from here 

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Player.h"
#include "Laser.h"
#include "TextManager.h"

// Setting basic view size. 
const float VIEW_WIDTH = 800.f;
const float VIEW_HEIGHT = 600.f;

//& to avoid expensive copy and pass by reference
// Placeholder functions
void handleEvents(sf::RenderWindow& window);
void transition(sf::RenderWindow& window, sf::Clock& clock, LevelManager& levelManager, int level);
void startScreen(sf::RenderWindow& window, sf::Font font);
void checkPlayerHealth(Player& player, sf::RenderWindow& window, sf::Font font);

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "SFML window");

    // Create the player
    Player player("./Assets/Sprites/Run-Sheet.png", 100);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("Assets/Alkatra-VariableFont_wght.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }


    TextManager textManager;
    textManager.loadFont("Assets/Alkatra-VariableFont_wght.ttf");


    // Spells
    std::vector<Spell> spells;

    // Level system
    LevelManager levelManager;
    levelManager.setTextManager(&textManager);
	levelManager.loadLevel(1);


    // Clock for delta time
    sf::Clock clock;

    // Music Setup
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Assets/arcade-70780.mp3"))
    {
        std::cerr << "Failed to load background music!" << std::endl;
    }
    // Set the music to loop and play it
    backgroundMusic.setLoop(true);   
    backgroundMusic.play();

    // Run the start screen function
	startScreen(window, font);

	// Primary game loop
    while (window.isOpen())
    {
		// Check the player's health
		checkPlayerHealth(player, window, font);

        // Handle events
		handleEvents(window);
        if (textManager.getScore() >= 500 && levelManager.getLevel() == 1) //changed 1000 to 500 for level change score
        {
            transition(window,clock,levelManager,2);
        }
        float dt = clock.restart().asSeconds();


        // Update player and converts pixel coordinates to world coordinates`
        // This is done so that the player can aim with the mouse
        // Get the mouse position in world coordinates
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        player.update(dt, spells, mouseWorldPos);

        // Update level
        levelManager.update(dt, player, spells);

        // Update Player Health
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        textManager.updatePlayerHealth(player.getHealth(), viewCenter, viewSize);
        textManager.updateScoreDisplay(viewCenter, viewSize);

        // Update view so that the camera will follow the player
        sf::View view = window.getView();
        view.setCenter(player.getPosition());
        window.setView(view);

        // Clear screen
        window.clear();

        // Draw everything
        levelManager.draw(window, font, player.getPosition());
        player.draw(window);
        textManager.draw(window);

        // Draw and update spells
        for (auto spell = spells.begin(); spell != spells.end();)
        {
            spell->update(dt);
            spell->draw(window);
            if (spell->isOutOfBounds())
                spell = spells.erase(spell);
            else
                ++spell;
        }

        // Display everything
        window.display();
    }

    return 0;
}


// level transition function. 
// Accepting a specefic level so that it can be used later if more levels are needed
void transition(sf::RenderWindow& window, sf::Clock& clock, LevelManager& levelManager, int level)
{
    float transitionTimer = 0.f;

    sf::RectangleShape blackOverlay(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
    blackOverlay.setFillColor(sf::Color::Black);
    blackOverlay.setPosition(0, 0);

    // Draw black screen overlay over everything
    window.clear();
    window.draw(blackOverlay);
    window.display();
	while (transitionTimer < 1.5f)
	{
        // Handle events during the transition because my attention to detail is screaming at me
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                blackOverlay.setSize(sf::Vector2f(event.size.width, event.size.height));
        }
		transitionTimer += clock.restart().asSeconds();
        // Center overlay on view
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        blackOverlay.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);

        window.clear();
        window.draw(blackOverlay);
        window.display();
	}
    levelManager.loadLevel(level);

}

// Function to manage the start screen. Giving instructions, and waiting for the player to press enter
void startScreen(sf::RenderWindow& window, sf::Font font)
{

	sf::Text startText("Press Enter to Start \n Press space to Fire", font, 50);
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
            // Otherwise end the start screen.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
				return; 
		}
		window.clear();
		window.draw(startText);
		window.display();
	}
}

// Function to check the players health, and trigger the end game. 
void checkPlayerHealth(Player& player, sf::RenderWindow& window, sf::Font font)
{
	if (player.getHealth() <= 0)
	{
		sf::Text gameOverText("Game Over \n Press esc to quit", font, 50);
		gameOverText.setFillColor(sf::Color::Red);
		// Set the position to centere on the screen
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        gameOverText.setPosition(
            viewCenter.x - gameOverText.getGlobalBounds().width / 2.f,
            viewCenter.y - gameOverText.getGlobalBounds().height / 2.f
        );

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window.close();
			}
			window.clear();
			window.draw(gameOverText);
			window.display();
		}
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