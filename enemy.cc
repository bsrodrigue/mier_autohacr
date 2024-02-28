#include "enemy.h"

Enemy create_base_enemy() {
  Enemy enemy;

  enemy.health = 3;
  enemy.shooting_interval = 1;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 3;
  enemy.type = BASE;
  enemy.state = ALIVE;

  return enemy;
}

Enemy create_enemy(Vector2 position, EnemyType type) {
  Enemy enemy;

  switch (type) {
  case BASE:
    enemy = create_base_enemy();
    break;
  }

  enemy.position = position;
  return enemy;
}
