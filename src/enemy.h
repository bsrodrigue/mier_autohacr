#include <raylib.h>

#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 50

typedef enum {
  BASE,
} EnemyType;

typedef enum {
  ALIVE,
  DEAD,
} EnemyState;

// TODO: Find a better way of handling flags
typedef struct {
  EnemyType type;
  EnemyState state;
  Vector2 position;
  float health;
  float max_health;
  float shooting_interval;
  float shooting_angle;
  float last_shot;
  float vision_radius;

  int projectile_rate;

  bool can_move;
  bool tracks_player;
  // Does not need to aim for player to shoot
  bool aimless_shooting;
  bool drops_items;
} Enemy;

Enemy create_enemy(Vector2 position, EnemyType type);

#endif
