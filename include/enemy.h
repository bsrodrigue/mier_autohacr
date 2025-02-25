#include "item_drop.h"
#include <raylib.h>

#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 50

struct Hitbox;

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

  int projectile_rate;    // How many projectiles per shot
  float projectile_angle; // Angle between each projectile
  float shooting_interval;
  float shooting_angle; // Where projectiles are pointing
  float last_shot;

  bool tracks_player;
  // Does not need to aim for player to shoot
  bool aimless_shooting;

  bool drops_items;
  ItemDrop item_drop;
} Enemy;

Enemy create_enemy(Vector2 position, EnemyType type, float enemy_health);

#endif
