#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>

class Player {
public:
  Player(std::string texturePath, float speed);
  void draw(sf::RenderWindow &window);
  void update(float dt);
  sf::Vector2f getPosition();
  sf::FloatRect getGlobalBounds();

private:
  sf::Texture texture;
  sf::Sprite sprite;
  float speed;

  void handleInput(float dt);
};

#endif
