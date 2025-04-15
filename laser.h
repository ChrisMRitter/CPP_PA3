// This represents a single spell, in this case, it will be a generic damage spell
// Header

// Spell.h
#ifndef SPELL_H
#define SPELL_H
#include <SFML/Graphics.hpp>

class Spell
{
public:
	// Constructor, x and y are the starting position, speed is the speed of the spell, direction is the direction of the spell
	// textureFile is the file path to the texture
	Spell(float x, float y, float speed, sf::Vector2f direction);
	// Function to update the spell
	void update(float dt);
	// Function to draw the spell
	void draw(sf::RenderWindow& window);
	// Function to get the position of the spell
	sf::Vector2f getPosition();
	// Function to check if the spell is out of bounds
	bool isOutOfBounds();
	// Function to get the global bounds of the attacks sprite
	sf::FloatRect getGlobalBounds();

private:
	// Texture for the spell
	sf::Texture texture;
	// Sprite for the spell
	sf::Sprite sprite; 
	// Speed of the spell
	float speed;
	// Direction of the spell
	sf::Vector2f direction; 
	// Distance traveled by the spell
	float distanceTraveled = 0.f;
	// Maximum distance the spell can travel
	float maxDistance = 1000.f; 
	// The visible part of the spell
	//sf::CircleShape circle;
	sf::RectangleShape square;
	//The new visible part of the spell




};

#endif