// This class will handle the players movement, and the input, 
// Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Spell.h"
#include "Hitbox.h"


#include <iostream>

class Player
{
public:
	Player(std::string texturePath, float speed);
	void draw(sf::RenderWindow& window);
	void update(float dt, std::vector<Spell>& spells, sf::Vector2f mousePos);

	// Handle the players input, passing in dt and the spells vector
	void handleInput(float dt,std::vector<Spell>& spells, sf::Vector2f mousePos);
	sf::Vector2f getPosition();
	// Get the players global 
	sf::FloatRect getGlobalBounds();
	// Function to make a spell
	void createSpell(std::vector<Spell>& spells, sf::Vector2f playerPos, sf::Vector2f fireDirection);
	int getHealth() const;
	void takeDamage(int amount);
	bool canTakeDamage() const;

	// Get hitbox
	const Hitbox& getHitbox() const;

private:
	sf::Texture texture;
	sf::Sprite sprite;
	int curentFrame = 0; // The current frame of the animation
	// Time per frame
	float timePerFrame = 0.2f; // 1/5 of a second
	// Time since the last frame of the animation
	float timeSinceLastFrame = 0.0f;
	// Speed of the player
	float speed = 100.f;
	// The health of the player
	int health = 10;
	// Damage management
	float damageCooldown = 0.5f; // seconds
	float timeSinceLastHit = 0.f;
	float spellCooldown = 0.5f;
	// The time since the last spell was cast
	float timeSinceLastSpell = 0.0f;

	void handleAnimation(int direction, float dt);

	Hitbox hitbox;

};

#endif