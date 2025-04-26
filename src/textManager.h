// The textManager class is intended to provide a singular class to manage the
// majority of text.

#pragma once
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

class textManager {
public:
  bool loadFont(const std::string &path);
  void updatePlayerHealth(int health, sf::Vector2f viewCenter,
                          sf::Vector2f viewSize);
  void draw(sf::RenderWindow &window);
  void addScore(int amount);
  void updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize);
  int getScore();

private:
  sf::Font font;
  sf::Text playerHealthText;
  int score = 0;
  sf::Text scoreText;
};

#endif
