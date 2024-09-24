#include "enemy.h"

// Find a way to not hardcode enemy types
Enemy create_base_enemy() {
  Enemy enemy;

  enemy.health = 50;
  enemy.max_health = 50;
  enemy.shooting_interval = 1;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 50;
  enemy.shooting_angle = 0;

  enemy.projectile_rate = 1;

  enemy.type = BASE;
  enemy.state = ALIVE;

  // Flags
  enemy.can_move = false;
  enemy.tracks_player = false;
  enemy.aimless_shooting = true;
  enemy.drops_items = true;
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
