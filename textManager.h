/*
Contributors: Alexander Aldama-Apodaca
*/

#pragma once
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

// Set up the textManager class - done by Alexander Aldama-Apodaca
class textManager {
public:
	// Constructor - done by Alexander Aldama-Apodaca
	bool loadFont(const std::string& path);
	// Function to update the player's health text - done by Alexander Aldama-Apodaca
	void updatePlayerHealth(int health, sf::Vector2f viewCenter, sf::Vector2f viewSize);
	// Function to draw the text - done by Alexander Aldama-Apodaca
	void draw(sf::RenderWindow& window);
	// Function to add score - done by Alexander Aldama-Apodaca
	void addScore(int amount);
	// Function to update the score display - done by Alexander Aldama-Apodaca
	void updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize);
	// Function to get the score - done by Alexander Aldama-Apodaca
	int getScore();
private:
	sf::Font font; // The font used by the game - done by Alexander Aldama-Apodaca
	sf::Text playerHealthText; // Text displaying the player's health - done by Alexander Aldama-Apodaca
	int score = 0; // The player's score, initially set to 0 - done by Alexander Aldama-Apodaca
	sf::Text scoreText; // Text displaying the player's score - done by Alexander Aldama-Apodaca
};

#endif
