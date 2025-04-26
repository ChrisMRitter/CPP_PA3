#include "enemyManager.h"
#include "hitbox.h"
#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager()
    : rng(std::random_device{}()), hpDist(baseHealth, baseHealth),
      speedDist(baseSpeed, baseSpeed) {}

void EnemyManager::configure(std::shared_ptr<sf::Texture> tex, int maxEn,
                             int health, float cooldown, float speed,
                             float scale) {
  enemyTexture = std::move(tex);
  enemyScale = scale;

  maxEnemies = maxEn;
  spawnCooldown = cooldown;
  baseHealth = health;
  baseSpeed = speed;

  hpDist = std::uniform_int_distribution<>(health, health);
  speedDist = std::uniform_real_distribution<float>(speed, speed);
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

int EnemyManager::handleLaserCollisions(std::vector<Laser> &lasers) {
  int kills = 0;
  for (auto it = lasers.begin(); it != lasers.end();) {
    bool hit = false;
    for (auto &e : enemies) {
      if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
        int remaining = e.takeDamage();
        if (remaining <= 0)
          ++kills;
        hit = true;
        break;
      }
    }
    if (hit)
      it = lasers.erase(it);
    else
      ++it;
  }
  return kills;
}

void EnemyManager::handlePlayerCollisions(Player &player, int damage) {
  for (auto &e : enemies) {
    if (e.getGlobalBounds().intersects(player.getHitbox().getBounds()))
      player.takeDamage(damage);
  }
}

void EnemyManager::spawnEnemy(const sf::Vector2f &) {
  float buffer = 50.f;
  float x = static_cast<float>(std::rand() % 800);
  sf::Vector2f pos(x, -buffer);

  int hp = hpDist(rng);
  float spd = speedDist(rng);
  enemies.emplace_back(pos.x, pos.y, spd, hp, enemyTexture, enemyScale);
}
