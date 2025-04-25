// represents a single laser projectile fired by player or enemies

#ifndef LASER_H
#define LASER_H
#include <SFML/Graphics.hpp>

class Laser {

public:
  // constructor: x & y start pos, laser speed, direction
  Laser(float x, float y, float speed, sf::Vector2f direction);

  void update(float dt); // update laser pos

  void draw(sf::RenderWindow &window); // draw laser

  sf::Vector2f getPosition(); // get pos

  bool isOutOfBounds(); // check if laser out of bounds

  sf::FloatRect getGlobalBounds(); // get global bounds for collision detection

private:
  float speed;

  sf::Vector2f direction;

  float distanceTraveled = 0.f;

  float maxDistance = 1000.f;

  sf::RectangleShape rectangle; // visible laser
};

#endif
