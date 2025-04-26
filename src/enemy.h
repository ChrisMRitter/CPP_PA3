// enemy.h
// This is the enemy class, holding its health, the sprite, and its movement.

#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <memory>

class Enemy {
public:
    // Constructor: x and y are the starting position, speed is the speed of the enemy, texture is the file path to the texture, scale is the scale of the enemy
    Enemy(float x,
          float y,
          float speed,
          int health,
          std::shared_ptr<sf::Texture> texture,
          float scale);

    // Function to update the enemy
    void update(float dt, sf::Vector2f playerPos);
    // Function to draw the enemy
    void draw(sf::RenderWindow& window);
    // Function to get the global bounds of the enemy sprite
    sf::FloatRect getGlobalBounds() const;
    // Function to get the health of the enemy
    int getHealth() const;
    // Function to set the health of the enemy
    void setHealth(int health);
    // Function to take damage
    int takeDamage();
    // Function to kill an enemy
    void killEnemy();
    // Function to check if the enemy is still alive
    bool isAlive() const;

private:
    // Texture for the enemy
    std::shared_ptr<sf::Texture> texture;
    // Sprite for the enemy
    sf::Sprite sprite;
    // Speed of the enemy
    float speed;
    // Health of the enemy
    int health;
    // Maximum health of the enemy
    int maxHealth;
    float scale;
    // Boolean to check if the enemy is alive or dead
    bool alive = true;
};

#endif
