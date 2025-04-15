// The spell class implementation
#include <SFML/Graphics.hpp>
#include "Laser.h"

#include <iostream>
#include <string>
using namespace sf;



Spell::Spell(float x, float y, float speed, sf::Vector2f direction)
{
	// A spell is a simple green ball here.
	//circle = sf::CircleShape(5.f);
	square = RectangleShape(sf::Vector2f(5.f, 5.f)); //changed from circle to square

	square.setFillColor(sf::Color::Red); //square version
	//circle.setFillColor(sf::Color::Red); //changed spell color from green to red, circles to square
	

	square.setOrigin(square.getSize().x / 2, square.getSize().y / 2);

	square.setPosition(x, y);
	//circle.setPosition(x, y);
	this->speed = speed;
	this->direction = direction;

	square.setScale(2.f, 2.f);
	//circle.setScale(2.f, 2.f);
}

// Function to update the spell
void Spell::update(float dt)
{
	// Move the spell in the direction it was fired
	
	square.move(direction.x * speed * dt, direction.y * speed * dt);
	//circle.move(direction.x * speed * dt, direction.y * speed * dt);
	distanceTraveled += speed * dt;
}

// Check if the spell is out of bounds (Or in this case, just if it has traveled far enough
bool Spell::isOutOfBounds()
{
	// Check if the spell has traveled more than the max distance
	if (distanceTraveled >= maxDistance)
	{
		return true;
	}
	return false;
}

// Function to draw the spell
void Spell::draw(sf::RenderWindow& window)
{
	window.draw(square);
	//window.draw(circle);
}
// Function to get the position of the spell
sf::Vector2f Spell::getPosition()
{
	return square.getPosition();
	//return circle.getPosition();
}

// Function to get the global bounds of the attacks sprite, 
sf::FloatRect Spell::getGlobalBounds()
{
	
	return square.getGlobalBounds();
	//return circle.getGlobalBounds();
}