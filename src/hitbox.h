// The Hitbox class is a custom-made hitbox for the player and enemies.

#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics.hpp>

class Hitbox {
public:
  // Constructor for the Hitbox class
  Hitbox(sf::Vector2f size = {40.f, 40.f},
         sf::Color color = sf::Color(0, 255, 0, 100));

  void setPosition(const sf::Vector2f &pos);
  void setSize(const sf::Vector2f &size);
  void setColor(const sf::Color &color);

  sf::FloatRect getBounds() const;
  void draw(sf::RenderWindow &window);

private:
  sf::RectangleShape box;
};

#endif
