/*
player.h - header file to manage game components relating to the player

Contributors: Alexander Aldama-Apodaca
*/

#pragma once
// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>

#include <iostream>

class Player {
public:
	Player(std::string texturePath, float speed);
	void draw(sf::RenderWindow& window);
	void update(float dt);
	sf::Vector2f getPosition();
	sf::FloatRect getGlobalBounds();
	int getHealth() const; // return player's health - done by Alexander Aldama-Apodaca

private:
	sf::Texture texture;
	sf::Sprite sprite;
	float speed;
	int health = 10; // The health of the player - done by Alexander Aldama-Apodaca

	void handleInput(float dt);
};

#endif // PLAYER_H
