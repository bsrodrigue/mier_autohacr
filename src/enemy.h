#include <raylib.h>

#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 50

typedef enum {
  BASE,
  SENTRY_A,
} EnemyType;

typedef enum {
  ALIVE,
  DEAD,
} EnemyState;

typedef struct {
  float health;
  float shooting_interval;
  float shooting_angle;
  float last_shot;
  Vector2 position;
  float vision_radius;
  EnemyType type;
  EnemyState state;
} Enemy;

Enemy create_enemy(Vector2 position, EnemyType type);

#endif
