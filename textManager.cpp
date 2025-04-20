#include "textManager.h"
#include <iostream>

// Constructor
bool textManager::loadFont(const std::string& path) {
	if (!font.loadFromFile(path)) {
		std::cerr << "Failed to load font: " << path << std::endl;
		return false;
	}
	playerHealthText.setFont(font);
	playerHealthText.setCharacterSize(24);
	playerHealthText.setFillColor(sf::Color::Red);
	playerHealthText.setStyle(sf::Text::Bold);
	return true;
}

// Function to update the player's health text
void textManager::updatePlayerHealth(int health, sf::Vector2f viewCenter, sf::Vector2f viewSize) {
	playerHealthText.setString("Health: " + std::to_string(health));
	playerHealthText.setPosition(viewCenter.x - playerHealthText.getGlobalBounds().width / 2.f, viewCenter.y - viewSize.y / 2.f + 10.f);
}

// Function to add score
void textManager::addScore(int amount) {
	score += amount;
}

// Function to get the score
int textManager::getScore() {
	return score;
}

// Function to update the score display
void textManager::updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize) {
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score: " + std::to_string(score));

	scoreText.setPosition(viewCenter.x - scoreText.getGlobalBounds().width / 2.f, viewCenter.y - viewSize.y / 2.f + 40.f); // below the health
}

// Function to draw the text
void textManager::draw(sf::RenderWindow& window) {
	window.draw(playerHealthText);
	window.draw(scoreText);
}
