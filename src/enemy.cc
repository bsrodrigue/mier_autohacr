#include "enemy.h"
#include "editor_entities.h"

// TODO: Find a more flexible way to handle dynamic entity creation
Enemy create_enemy_from_level_data(Vector2 position,
                                   const EditorEnemy &editor_enemy) {

  Enemy enemy;

  enemy.position = position;
  enemy.health = editor_enemy.enemy_health;
  enemy.max_health = enemy.health;
  enemy.shooting_interval = editor_enemy.shooting_interval;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 50;
  enemy.shooting_angle = 0;

  enemy.projectile_rate = 1;

  enemy.type = BASE_ENEMY;
  enemy.state = ALIVE;

  // Flags
  enemy.can_move = false;
  enemy.tracks_player = editor_enemy.tracks_player;
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
