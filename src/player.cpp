// This class handles horizontal movement of a spaceship sprite

#include "player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

Player::Player(std::string texturePath, float speed) {
  // Load the texture from the file
  if (!texture.loadFromFile(texturePath)) {
    std::cerr << "Error loading texture!" << std::endl;
  }

  // Set the texture to the sprite
  sprite.setTexture(texture);
  sprite.setTextureRect(sf::IntRect(0, 50, 256, 256)); // Adjust if needed

  // Position and scale
  sprite.setPosition(400, 500);
  auto bounds = sprite.getLocalBounds();
  sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  sprite.setScale(.5, .5); // Scale up the sprite

  this->speed = speed;
}

// Get spaceship's current position
sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

void Player::draw(sf::RenderWindow &window) { window.draw(sprite); }

void Player::update(float dt) { handleInput(dt); }

void Player::handleInput(float dt) {
  sf::Vector2f movement(0, 0);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    movement.x -= speed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    movement.x += speed;
  }

  sprite.move(movement * dt);
}

// Get the player's health
int Player::getHealth() const { return health; }
