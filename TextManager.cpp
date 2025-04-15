#include "textmanager.h"
#include <iostream>
// This class is intended to provide a singular class to manage the majority of text. 
// With time, this could be made more modular, and fit to each of the different text options this game requires.


// Constructor
bool TextManager::loadFont(const std::string& path)
{
    if (!font.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << path << std::endl;
        return false;
    }

    playerHealthText.setFont(font);
    playerHealthText.setCharacterSize(24);
    playerHealthText.setFillColor(sf::Color::Red);
    playerHealthText.setStyle(sf::Text::Bold);
    return true;
}

// Function to update the players health text
void TextManager::updatePlayerHealth(int health, sf::Vector2f viewCenter, sf::Vector2f viewSize)
{
    playerHealthText.setString("Health: " + std::to_string(health));
    playerHealthText.setPosition(
        viewCenter.x - playerHealthText.getGlobalBounds().width / 2.f,
        viewCenter.y - viewSize.y / 2.f + 10.f
    );
}

// Function to add score
void TextManager::addScore(int amount)
{
    score += amount;
}

// Function to get the score
int TextManager::getScore()
{
	return score;
}

// Function to update the score display
void TextManager::updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize)
{
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: " + std::to_string(score));

    scoreText.setPosition(
        viewCenter.x - scoreText.getGlobalBounds().width / 2.f,
        viewCenter.y - viewSize.y / 2.f + 40.f // below the health
    );
}

// Function to draw the text
void TextManager::draw(sf::RenderWindow& window)
{
    window.draw(playerHealthText);
    window.draw(scoreText);
}
