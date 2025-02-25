#include "enemy.h"

// Find a way to not hardcode enemy types
Enemy create_base_enemy(float enemy_health) {
  Enemy enemy;

  enemy.health = enemy_health;
  enemy.max_health = enemy.health;
  enemy.shooting_interval = 0.01;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 50;
  enemy.shooting_angle = 0;

  enemy.projectile_rate = 1;

  enemy.type = BASE_ENEMY;
  enemy.state = ALIVE;

  // Flags
  enemy.can_move = false;
  enemy.tracks_player = false;
  enemy.aimless_shooting = true;

  // Item Dropping
  enemy.drops_items = true;

  ItemDrop item_drop;

  item_drop.item_effect = KEY_EFFECT;
  item_drop.item_usage = INVENTORY_USAGE;
  item_drop.texture = KEY_TEXTURE;
  item_drop.count = 10;

  enemy.item_drop = item_drop;

  return enemy;
}

//TODO: Find a more flexible way to handle dynamic entity creation
Enemy create_enemy(Vector2 position, EnemyType type, float enemy_health) {
  Enemy enemy;

  switch (type) {
  case BASE_ENEMY:
    enemy = create_base_enemy(enemy_health);
    break;
  }

  enemy.position = position;
  return enemy;
}
