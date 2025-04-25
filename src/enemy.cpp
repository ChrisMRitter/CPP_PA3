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
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setOrigin(32, 32);
    sprite.setPosition(x, y);
    sprite.setScale(scale, scale);
    sprite.setRotation(180.f);
}

void Enemy::update(float dt, sf::Vector2f playerPos)
{
    sf::Vector2f dirVec = playerPos - sprite.getPosition();
    float len = std::sqrt(dirVec.x*dirVec.x + dirVec.y*dirVec.y);
    if (len > 0.f) {
        dirVec /= len;
        sprite.move(dirVec * speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Enemy::getGlobalBounds() const { return sprite.getGlobalBounds(); }
int Enemy::getHealth() const { return health; }
void Enemy::setHealth(int h) { health = h; }
int Enemy::takeDamage() { health--; if (health <= 0) alive = false; return health; }
void Enemy::killEnemy() { alive = false; }
bool Enemy::isAlive() const { return alive; }
