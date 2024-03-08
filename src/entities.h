#include <raylib.h>
#include <vector>

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum { BREAKABLE, UNBREAKABLE } WallType;

typedef enum {
  EMPTY,
  UBWALL,
  BWALL,
  PLAYER,
  BASE_ENEMY,
  SENTRY_A_ENEMY,
  WARPZONE
} EntityType;

typedef struct {
  WallType type;
  float health;
  Vector2 position;
} Wall;

#endif
