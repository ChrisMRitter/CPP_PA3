// enemyManager.cpp
// enemyManager implementation
#include "enemyManager.h"
#include "hitbox.h"
#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager()
    : rng(std::random_device{}()), hpDist(baseHealth, baseHealth),
      speedDist(baseSpeed, baseSpeed) {}

// Function to configure the enemy manager, letting us change how we are creating enemies as need be
void EnemyManager::configure(std::shared_ptr<sf::Texture> tex, int maxEn,
                             int health, float cooldown, float speed,
                             float scale) {
  enemyTexture = std::move(tex);
  enemyScale = scale;

  maxEnemies = maxEn;
  spawnCooldown = cooldown;
  baseHealth = health;
  baseSpeed = speed;

    // reset distributions to fixed values
  hpDist = std::uniform_int_distribution<>(health, health);
  speedDist = std::uniform_real_distribution<float>(speed, speed);
}

// Function to move and remove enemies
void EnemyManager::update(float dt, const sf::Vector2f &playerPos) {
  spawnTimer += dt;
    // Spawn logic
  if (spawnTimer >= spawnCooldown && enemies.size() < (size_t)maxEnemies) {
    spawnEnemy(playerPos);
    spawnTimer = 0.f;
  }

    // Update all enemies
  for (auto &e : enemies)
    e.update(dt, playerPos);

    // Remove enemies that are marked as dead
  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [](const Enemy &e) { return !e.isAlive(); }),
                enemies.end());
}

// Function to draw all enemies
void EnemyManager::draw(sf::RenderWindow &window) {
  for (auto &e : enemies)
    e.draw(window);
}

// Function to handle collisions between lasers and enemies
int EnemyManager::handleLaserCollisions(std::vector<Laser> &lasers) {
  int kills = 0;
  for (auto it = lasers.begin(); it != lasers.end();) {
    bool hit = false;
    for (auto &e : enemies) {
        // For sprite to sprite collision, just use getGlobalBounds() for both parts
      if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
        int remaining = e.takeDamage(); // reduce the enemy's health
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

// Function to check for collisions between the player and enemies
void EnemyManager::handlePlayerCollisions(Player &player, int damage) {
  for (auto &e : enemies) {
    if (e.getGlobalBounds().intersects(player.getHitbox().getBounds()))
      player.takeDamage(damage);
  }
}

// Function to spawn an enemy at a somewhat random location around the player
void EnemyManager::spawnEnemy(const sf::Vector2f &) {
    // Only from top of screen (hardcoded width=800px)
  float buffer = 50.f;
  float x = static_cast<float>(std::rand() % 800);
  sf::Vector2f pos(x, -buffer);

    // fixed attributes
  int hp = hpDist(rng);
  float spd = speedDist(rng);
  enemies.emplace_back(pos.x, pos.y, spd, hp, enemyTexture, enemyScale);
}
