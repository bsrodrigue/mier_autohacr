#include <raylib.h>
#include <vector>

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum { BREAKABLE, UNBREAKABLE } WallType;

typedef enum { EMPTY, UBWALL, BWALL, PLAYER, BASE_ENEMY } EntityType;

typedef struct {
  WallType type;
  float health;
  Vector2 position;
} Wall;

typedef struct {
  Vector2 position;
  Vector2 velocity;
  Vector2 direction;
  float angle;
  std::vector<Vector2> shape;
} GameObject;

#endif
