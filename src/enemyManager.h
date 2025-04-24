#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

class EnemyManager {
public:
  EnemyManager();

  // Provide a cropped texture, spawn params, dynamic attribute ranges, and a
  // draw scale
  void configure(std::shared_ptr<sf::Texture> tex, int maxEnemies, int health,
                 float cooldown, float speed, float scale = 1.f);

  void update(float dt, const sf::Vector2f &playerPos);
  void draw(sf::RenderWindow &window);

private:
  std::vector<Enemy> enemies;
  std::shared_ptr<sf::Texture> enemyTexture;
  float enemyScale = 1.f;

  float spawnTimer = 0.f;
  float spawnCooldown = 2.f;
  int maxEnemies = 10;

  // Base attributes (used as mean values)
  int baseHealth = 3;
  float baseSpeed = 60.f;

  // Random generators for per-instance variation
  std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> hpDist;
  std::uniform_real_distribution<float> speedDist;
  std::uniform_real_distribution<float> scaleDist;

  void spawnEnemy(const sf::Vector2f &playerPos);
};

#endif
