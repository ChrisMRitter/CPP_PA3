// player.cpp
// The Player class handles horizontal movement of a spaceship sprite

#include "player.h"
#include "gameconstants.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

Player::Player(std::string texturePath, float speed) {
  // Load the texture from the file
  if (!texture.loadFromFile(texturePath)) {
    std::cerr << "Error loading texture!" << texturePath << std::endl;
    // Set the texture rect to the single base sprite
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setColor(sf::Color::Blue);
  } else {
    std::cout << "Loaded texture with size: " << texture.getSize().x << "x"
              << texture.getSize().y << std::endl;
    // Set the texture to the sprite
    sprite.setTexture(texture);
  }

  // Position at bottom center of screen
  sprite.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f);
  // Make sure origin is at center of sprite for proper rotation and movement
  sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
  // Use a reasonable scale
  sprite.setScale(0.5f, 0.5f);

  // Create a smaller hitbox that better matches the ship
  hitbox = new Hitbox(sf::Vector2f(64.f, 32.f));
  // Set the speed of the player
  this->speed = speed;

  if (laserBuffer.loadFromFile("./Assets/Sound/lasersound.mp3")) {
    laserSound.setBuffer(laserBuffer);
  }
}

// Destructor
Player::~Player() { delete hitbox; }

// Get the spaceship's current position
sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

// Get the player's global bounds
sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

// Draw the player
void Player::draw(sf::RenderWindow &window) { window.draw(sprite); }

// Function to handle all the things the player needs to update
void Player::update(float dt, std::vector<Laser> &lasers,
                    sf::Vector2f mousePos) {
  timeSinceLastHit += dt; // invulnerability cooldown iterator
  timeSinceLastLaser += dt; // laser cooldown iterator

  handleInput(dt, lasers, mousePos);
  // Set the hitbox's position to the player's position
  hitbox->setPosition(sprite.getPosition());
}

// Function to handle input from the player
void Player::handleInput(float dt, std::vector<Laser> &lasers,
                         sf::Vector2f mousePos) {
  // Handle the input
  sf::Vector2f movement(0, 0);
  sf::Vector2f position = sprite.getPosition();
  float halfWidth = sprite.getGlobalBounds().width / 2.f;

  // Moving left
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    // Don't allow player to move past left edge of screen
    if (position.x - halfWidth > 0) {
      movement.x -= speed;
    }
  }
  // Moving right
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    // Don't allow player to move past right edge of screen
    if (position.x + halfWidth < VIEW_WIDTH) {
      movement.x += speed;
    }
  }
  // Apply movement
  sprite.move(movement * dt);

  // If the spacebar is pressed, create a laser, and fire it at the mouse
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
      timeSinceLastLaser > laserCooldown) {
    // Get the direction to fire the laser
    sf::Vector2f fireDirection = mousePos - sprite.getPosition();
    float length = std::sqrt(fireDirection.x * fireDirection.x +
                             fireDirection.y * fireDirection.y);
    if (length > 0) {
      fireDirection /= length;
    }
    createLaser(lasers, sprite.getPosition(), fireDirection);
    laserSound.play();
    // Reset the time since the last laser was fired
    timeSinceLastLaser = 0;
  }
}

// Function to create a laser
void Player::createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                         sf::Vector2f fireDirection) {
  // Create a new laser and add it to the vector
  lasers.push_back(Laser(playerPos.x, playerPos.y, 1000.f, fireDirection));
}

// Get the player's health
int Player::getHealth() const { return health; }

// Get the player's hitbox
const Hitbox &Player::getHitbox() const { return *hitbox; }

// Check if the player can take damage
bool Player::canTakeDamage() const {
  return timeSinceLastHit >= damageCooldown;
}

// Damage the player
void Player::takeDamage(int amount) {
  if (canTakeDamage()) {
    health -= amount;
    timeSinceLastHit = 0.f;
  }
}

void Player::handleAnimation(int /*direction*/, float /*dt*/) {}

