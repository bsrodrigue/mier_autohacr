#pragma once

#include "item_drop.h"
#include <raylib.h>

#define MAX_ENEMIES 500

struct Hitbox;
struct EditorEnemy;

typedef enum {
  BASE_ENEMY,
} EnemyType;

typedef enum {
  ALIVE,
  DEAD,
  PATROLLING,
} EnemyState;

typedef enum {

} PatrolingStrategy;

// TODO: Find a better way of handling flags
typedef struct {
  // Health
  float health;
  float max_health;

  // Position
  Vector2 position;

  // Movement
  bool can_move; // (mobile/immobile)
  bool can_chase;

  bool is_mobile;
  bool is_patrolling;
  bool is_chasing;

  // Player Tracking
  bool is_tracking;
  bool can_track;

  float vision_radius;

  EnemyType type;
  EnemyState state;

  // Shooting Behaviour
  int projectile_rate;    // How many projectiles per shot
  float projectile_angle; // Angle between each projectile

  float shooting_interval;
  float last_shot_time; // For continuous shooting
  float first_shot_time;
  float last_cooldown_time;

  float shooting_duration;          // Shoot during given time
  float shooting_cooldown_duration; // Cooldown before shooting again

  float shooting_angle; // Where projectiles are pointing

  bool tracks_player;
  bool is_first_shot;
  // Does not need to aim for player to shoot
  bool aimless_shooting;

  bool drops_items;
  ItemDrop item_drop;
} Enemy;

Enemy create_enemy_from_level_data(Vector2 position,
                                   const EditorEnemy &editor_enemy);
