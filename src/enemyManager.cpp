#include "enemyManager.h"
#include <algorithm>
#include <iostream>

EnemyManager::EnemyManager()
    : hpDist(baseHealth / 2, baseHealth * 2),
      speedDist(baseSpeed * 0.5f, baseSpeed * 1.5f),
      scaleDist(enemyScale * 0.5f, enemyScale * 1.5f) {}

void EnemyManager::configure(std::shared_ptr<sf::Texture> tex, int maxEnemies,
                             int health, float cooldown, float speed,
                             float scale) {
  enemyTexture = std::move(tex);
  enemyScale = scale;

  this->maxEnemies = maxEnemies;
  this->spawnCooldown = cooldown;
  this->baseHealth = health;
  this->baseSpeed = speed;

  // reset distributions with updated base values
  hpDist = std::uniform_int_distribution<int>(health / 2, health * 2);
  speedDist = std::uniform_real_distribution<float>(speed * 0.5f, speed * 1.5f);
  scaleDist = std::uniform_real_distribution<float>(scale * 0.5f, scale * 1.5f);
}

void EnemyManager::update(float dt, const sf::Vector2f &playerPos) {
  spawnTimer += dt;
  if (spawnTimer >= spawnCooldown && enemies.size() < (size_t)maxEnemies) {
    spawnEnemy(playerPos);
    spawnTimer = 0.f;
  }
  for (auto &e : enemies)
    e.update(dt, playerPos);

  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [](const Enemy &e) { return !e.isAlive(); }),
                enemies.end());
}

void EnemyManager::draw(sf::RenderWindow &window) {
  for (auto &e : enemies)
    e.draw(window);
}

void EnemyManager::spawnEnemy(const sf::Vector2f & /*playerPos*/) {
  float buffer = 50.f; // how far off-screen above
  float x = static_cast<float>(std::rand() % 800);
  float y = -buffer; // just above the top edge

  enemies.emplace_back(x, y,
                       baseSpeed,  // fixed speed
                       baseHealth, // fixed health
                       enemyTexture,
                       enemyScale // fixed draw scale
  );
}
