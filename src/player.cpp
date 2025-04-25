// This class handles horizontal movement of a spaceship sprite

#include "player.h"
#include "gameconstants.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

Player::Player(std::string texturePath, float speed) {
  // Load the texture from the file
  if (!texture.loadFromFile(texturePath)) {
    std::cerr << "Error loading texture!" << texturePath << std::endl;
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setColor(sf::Color::Blue);

  }

  else {
    std::cout << "Loaded texture with size: " << texture.getSize().x << "x"
              << texture.getSize().y << std::endl;

    sprite.setTexture(texture);
  }

  // Position at bottom center of screen
  sprite.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f);

  // Make sure origin is at center of sprite for proper rotation and movement
  sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

  // Use a reasonable scale
  sprite.setScale(0.5f, 0.5f); // 0.5f for both later

  // Create a smaller hitbox that better matches the ship
  hitbox = new Hitbox(sf::Vector2f(64.f, 32.f)); // 50.f, 25.f

  this->speed = speed;
}

// Destructor
Player::~Player() { delete hitbox; }

// Get spaceship's current position
sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

void Player::draw(sf::RenderWindow &window) {
  window.draw(sprite);
  // hitbox->draw(window); -uncomment for hitbox debugging
}

void Player::update(float dt, std::vector<Laser> &lasers,
                    sf::Vector2f mousePos) {
  timeSinceLastHit += dt;   // invuln cooldown iterator
  timeSinceLastLaser += dt; // laser cooldown iterator

  handleInput(dt, lasers, mousePos);

  hitbox->setPosition(sprite.getPosition());
}

void Player::handleInput(float dt, std::vector<Laser> &lasers,
                         sf::Vector2f mousePos) {

  // Handle the input
  sf::Vector2f movement(0, 0);

  sf::Vector2f position = sprite.getPosition();
  float halfWidth = sprite.getGlobalBounds().width / 2.f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) // moving left
  {
    // Don't allow player to move past left edge of screen
    if (position.x - halfWidth > 0) {
      movement.x -= speed;
    }
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // Moving Right
  {
    // Don't allow player to move past right edge of screen
    if (position.x + halfWidth < VIEW_WIDTH) {
      movement.x += speed;
    }
  }
  // apply movement
  sprite.move(movement * dt);

  // if the space bar is pressed, create a laser, and fire it at the mouse
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
      timeSinceLastLaser > laserCooldown) {
    // Get the direction to fire the laser
    sf::Vector2f fireDirection = mousePos - sprite.getPosition();

    float length = sqrt(fireDirection.x * fireDirection.x +
                        fireDirection.y * fireDirection.y);
    if (length > 0) {
      fireDirection /= length;
    }

    createLaser(lasers, sprite.getPosition(),
                fireDirection); // Changed from createSpell
    // Reset the time since the last laser was fired
    timeSinceLastLaser = 0; // Changed from timeSinceLastSpell
  }

  // placeholder for if we switch to a sprite sheet
  // handleAnimation(direction, dt);
}
// Function to create a laser
void Player::createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                         sf::Vector2f fireDirection) {
  // Create a new laser and add it to the vector
  lasers.push_back(Laser(playerPos.x, playerPos.y, 1000.f, fireDirection));
}

// Get the player's health
int Player::getHealth() const { return health; }

const Hitbox &Player::getHitbox() const {
  return *hitbox; // Dereference pointer
}

bool Player::canTakeDamage() const {
  return timeSinceLastHit >= damageCooldown; // shorthand if logic
}

void Player::takeDamage(int amount) {
  if (canTakeDamage()) {
    health -= amount;
    timeSinceLastHit = 0.f;
    // TODO: visual feedback code
  }
}

// Function to handle the animation of the player
void Player::handleAnimation(int direction, float dt) {
  // Increment the elapsed time
  timeSinceLastFrame += dt;

  // If the elapsed time is greater than the time per frame
  if (timeSinceLastFrame > timePerFrame) {
    if (curentFrame == 6) {
      curentFrame = 0;
    }
    // Set the texture rect to the current frame
    sprite.setTextureRect(sf::IntRect(curentFrame * 64, 0, 64, 64));

    // Handle the animation based on the direction
    // 1 being moving right and -1 being moving left
    sprite.setScale(2 * direction, 2);

    // Increment the current frame
    curentFrame++;

    // Reset the elapsed time
    timeSinceLastFrame = 0;
  }
}
