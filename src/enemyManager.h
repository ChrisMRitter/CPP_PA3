#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "laser.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

class EnemyManager {
public:
  EnemyManager();

  // tex       – pointer to the enemy sprite
  // maxEn     – max at once
  // health    – fixed HP per spawn
  // cooldown  – seconds between spawns
  // speed     – fixed movement speed
  // scale     – fixed draw scale
  void configure(std::shared_ptr<sf::Texture> tex, int maxEn, int health,
                 float cooldown, float speed, float scale = 1.f);

  // Move & cull
  void update(float dt, const sf::Vector2f &playerPos);
  // Draw all enemies
  void draw(sf::RenderWindow &window);

  // Laser collisions: lasers that hit enemies get erased; enemies lose health.
  // Returns number of enemies killed this frame.
  int handleLaserCollisions(std::vector<Laser> &lasers);

  // Player collisions: if any enemy overlaps player, deal `damage` HP.
  void handlePlayerCollisions(Player &player, int damage);

private:
  std::vector<Enemy> enemies;
  std::shared_ptr<sf::Texture> enemyTexture;
  float enemyScale = 1.f;

  float spawnTimer = 0.f;
  float spawnCooldown = 2.f;
  int maxEnemies = 10;

  int baseHealth = 3;
  float baseSpeed = 60.f;

  std::mt19937 rng;
  std::uniform_int_distribution<> hpDist;
  std::uniform_real_distribution<float> speedDist;

  void spawnEnemy(const sf::Vector2f &);
};

#endif
