#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>

#include <iostream>
#include "laser.h"

class Hitbox;
class Player {
public:
	Player(std::string texturePath, float speed);
	~Player();

	const Hitbox& getHitbox() const;
	void takeDamage(int amount);
	bool canTakeDamage() const;
	void handleInput(float dt, std::vector<Laser>& lasers, sf::Vector2f mousePos);

	void draw(sf::RenderWindow& window);
	void update(float dt, std::vector<Laser>& lasers, sf::Vector2f mousePos);
	sf::Vector2f getPosition();
	sf::FloatRect getGlobalBounds();
	int getHealth() const;
	void createLaser(std::vector<Laser>& lasers, sf::Vector2f playerPos, sf::Vector2f fireDirection);


private:

	Hitbox* hitbox;

	float damageCooldown = 0.5f; //half a second cooldown
	float timeSinceLastHit = 0.f;

	sf::Texture texture;
	sf::Sprite sprite;
	float speed = 100.f;
	// The health of the player
	int health = 10;
	int curentFrame = 0;
    float timePerFrame = 0.2f;
    float timeSinceLastFrame = 0.0f;
	float laserCooldown = 0.5f;
	float timeSinceLastLaser = 0.0f;


	int direction = 1; //the direction player is facing, 1 = right, -1 = left
	void handleAnimation(int direction, float dt);

	
};

#endif // PLAYER_H
