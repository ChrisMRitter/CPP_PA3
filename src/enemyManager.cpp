// enemyManager.cpp
#include "enemyManager.h"
#include "hitbox.h"
#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager()
  : rng(std::random_device{}()),
    hpDist(baseHealth, baseHealth),           // no random HP
    speedDist(baseSpeed, baseSpeed)           // no random speed
{}

void EnemyManager::configure(std::shared_ptr<sf::Texture> tex,
                             int maxEn,
                             int health,
                             float cooldown,
                             float speed,
                             float scale)
{
    enemyTexture = std::move(tex);
    enemyScale   = scale;

    maxEnemies    = maxEn;
    spawnCooldown = cooldown;
    baseHealth    = health;
    baseSpeed     = speed;

    // reset distributions to fixed values
    hpDist    = std::uniform_int_distribution<>(health, health);
    speedDist = std::uniform_real_distribution<float>(speed, speed);
}

void EnemyManager::update(float dt, const sf::Vector2f& playerPos)
{
    spawnTimer += dt;
    if (spawnTimer >= spawnCooldown && enemies.size() < (size_t)maxEnemies) {
        spawnEnemy(playerPos);
        spawnTimer = 0.f;
    }

    for (auto& e : enemies)
        e.update(dt, playerPos);

    // cull dead
    enemies.erase(
      std::remove_if(enemies.begin(), enemies.end(),
                     [](const Enemy& e){ return !e.isAlive(); }),
      enemies.end());
}

void EnemyManager::draw(sf::RenderWindow& window)
{
    for (auto& e : enemies)
        e.draw(window);
}

void EnemyManager::handleLaserCollisions(std::vector<Laser>& lasers)
{
    for (auto it = lasers.begin(); it != lasers.end();) {
        bool hit = false;
        for (auto& e : enemies) {
            if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
                e.takeDamage();  // subtract 1 HP (or however your Enemy does)
                hit = true;
                break;
            }
        }
        if (hit) it = lasers.erase(it);
        else     ++it;
    }
}

void EnemyManager::handlePlayerCollisions(Player& player, int damage)
{
    for (auto& e : enemies) {
        if (e.getGlobalBounds().intersects(player.getHitbox().getBounds())) {
            player.takeDamage(damage);
        }
    }
}

void EnemyManager::spawnEnemy(const sf::Vector2f&)
{
    // only from top of screen (hardcoded width=800px)
    float buffer = 50.f;
    float x = static_cast<float>(std::rand() % 800);
    sf::Vector2f pos(x, -buffer);

    // fixed attributes
    int   hp  = hpDist(rng);
    float spd = speedDist(rng);
    float sc  = enemyScale;

    enemies.emplace_back(
        pos.x, pos.y,
        spd,
        hp,
        enemyTexture,
        sc
    );
}

