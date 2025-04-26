// The Laser class represents a single laser projectile fired by player or
// enemies

#ifndef LASER_H
#define LASER_H
#include <SFML/Graphics.hpp>

class Laser {

public:
  // Constructor: x and y are the starting position, speed is the speed of the
  // laser, and direction is the direction of the laser
  Laser(float x, float y, float speed, sf::Vector2f direction);
  // Function to update the laser position
  void update(float dt);
  // Function to draw the laser
  void draw(sf::RenderWindow &window);
  // Function to get the position of the laser
  sf::Vector2f getPosition();
  // Function to check if the laser is out of bounds.
  bool isOutOfBounds();
  // Function to get the global bounds for collision detection
  sf::FloatRect getGlobalBounds();

private:
  // Speed of the laser
  float speed;
  // Direction of the laser
  sf::Vector2f direction;
  // Distance traveled by the laser
  float distanceTraveled = 0.f;
  // Maximum distance the laser can travel
  float maxDistance = 1000.f;
  // The visible part of the laser
  sf::RectangleShape rectangle;
};

#endif
