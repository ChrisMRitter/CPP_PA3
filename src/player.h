//player.h
// The Player class handles the player's movement and input

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "laser.h"

class Hitbox;
class Player {
public:
  Player(std::string texturePath, float speed);
  ~Player();

  // Get the player's hitbox
  const Hitbox &getHitbox() const;
  // Function to take damage
  void takeDamage(int amount);
  // Function to check if the player can take damage
  bool canTakeDamage() const;
  // Function to handle input from the player
  void handleInput(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);

  // Function to draw the player
  void draw(sf::RenderWindow &window);
  // Function to update the player
  void update(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);
  // Function to get the player's position
  sf::Vector2f getPosition();
  // Function to get the player's global bounds
  sf::FloatRect getGlobalBounds();
  // Function to get the player's health
  int getHealth() const;
  // Function to create a laser
  void createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                   sf::Vector2f fireDirection);

private:
  Hitbox *hitbox;

  float damageCooldown = 0.5f; // 1/2 second cooldown
  float timeSinceLastHit = 0.f;

  // Texture for the player
  sf::Texture texture;
  // Sprite for the player
  sf::Sprite sprite;
  // The player's speed
  float speed = 100.f;
  // The player's health
  int health = 10;
  // The current frame of the animation
  int curentFrame = 0;
  // Time for each frame of the animation
  float timePerFrame = 0.2f; // 1/5 of a second
  // Time since the last frame of the animation
  float timeSinceLastFrame = 0.0f;
  // Laser management
  float laserCooldown = 0.5f;
  // Time since the last laser was shot
  float timeSinceLastLaser = 0.0f;

  int direction = 1; // the direction the player is facing 1 = right, -1 = left

  sf::SoundBuffer laserBuffer;
  sf::Sound laserSound;

  void handleAnimation(int direction, float dt);
};

#endif
