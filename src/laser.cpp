// The laser projectile implementation
#include "laser.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;

// Constructor
Laser::Laser(float x, float y, float speed, sf::Vector2f direction) {
  // Create a red rectangular laser beam
  rectangle = RectangleShape(sf::Vector2f(10.f, 5.f));
  rectangle.setFillColor(sf::Color::Red);

  // Center the origin of the rectangle
  rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);

  // Set initial position
  rectangle.setPosition(x, y);

  // Store speed and direction
  this->speed = speed;
  this->direction = direction;

  // Rotate the laser to match its movement direction
  float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
  rectangle.setRotation(angle);
}

// Function to update the laser
void Laser::update(float dt) {
  // Move the laser in the direction it was fired
  rectangle.move(direction.x * speed * dt, direction.y * speed * dt);

  // Update distance traveled
  distanceTraveled += speed * dt;
}

// Check if the laser is out of bounds or traveled too far
bool Laser::isOutOfBounds() {
  // Check if the laser has traveled more than the max distance
  return distanceTraveled >= maxDistance;
}

// Function to draw the laser
void Laser::draw(sf::RenderWindow &window) { window.draw(rectangle); }

// Function to get the position of the laser
sf::Vector2f Laser::getPosition() { return rectangle.getPosition(); }

// Function to get the global bounds for collision detection
sf::FloatRect Laser::getGlobalBounds() { return rectangle.getGlobalBounds(); }
