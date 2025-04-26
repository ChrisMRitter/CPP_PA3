// enemy implementation

#include "enemy.h"
#include <cmath>

Enemy::Enemy(float x,
             float y,
             float speed,
             int health,
             std::shared_ptr<sf::Texture> texture,
             float scale)
    : texture(texture), speed(speed), health(health),
      maxHealth(health), scale(scale)
{
    sprite.setTexture(*texture); // Dereference shared_ptr
    // Set the texture rect to the single base sprite
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    // Set the origin of the sprite to the center of the sprite
    sprite.setOrigin(32, 32);
    sprite.setPosition(x, y);
    sprite.setScale(scale, scale);
    sprite.setRotation(180.f);
}

// Function to update the enemy
void Enemy::update(float dt, sf::Vector2f playerPos)
{
    // Get the direction the enemy should move in
    sf::Vector2f dirVec = playerPos - sprite.getPosition();
    // Move the enemy in the direction of the player after normalizing the vector
    float len = std::sqrt(dirVec.x*dirVec.x + dirVec.y*dirVec.y);
    if (len > 0.f) {
        dirVec /= len; // Normalize the direction
        sprite.move(dirVec * speed * dt);
    }
}

// Function to draw the enemy
void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

// Function to get the global bounds of the enemy's sprite
sf::FloatRect Enemy::getGlobalBounds() const { return sprite.getGlobalBounds(); }
// Function to get the health of the enemy
int Enemy::getHealth() const { return health; }
// Function to set the health of the enemy
void Enemy::setHealth(int h) { health = h; }
// Function to take damage
int Enemy::takeDamage() { health--; if (health <= 0) alive = false; return health; }
// Function to kill an enemy
void Enemy::killEnemy() { alive = false; }
// Function to check if an enemy is still alive
bool Enemy::isAlive() const { return alive; }
