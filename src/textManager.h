// textManager.h
// The textManager class is intended to provide a
// singular class to manage the majority of text.

#pragma once
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

class textManager {
public:
  // Function to load font
  bool loadFont(const std::string &path);
  // Function to update the player's health text
  void updatePlayerHealth(int health, sf::Vector2f viewCenter,
                          sf::Vector2f viewSize);
  // Function to draw the text
  void draw(sf::RenderWindow &window);
  // Function to add to the score
  void addScore(int amount);
  // Function to update the score display
  void updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize);
  // Function to get the score
  int getScore();

private:
  sf::Font font;
  sf::Text playerHealthText;
  int score = 0;
  sf::Text scoreText;
};

#endif
