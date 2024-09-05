#include "enemy.h"

// Find a way to not hardcode enemy types
Enemy create_base_enemy() {
  Enemy enemy;

  // TODO: Implement healthbar
  enemy.health = 50;
  enemy.max_health = 50;
  enemy.shooting_interval = 0.1;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 50;
  enemy.type = BASE;
  enemy.state = ALIVE;
  enemy.can_move = false;
  enemy.tracks_player = false;
  enemy.aimless_shooting = true;
  enemy.shooting_angle = 0;
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
