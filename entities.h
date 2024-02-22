#include <raylib.h>
#include <vector>

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum { BREAKABLE, UNBREAKABLE } WallType;

typedef enum { EMPTY, UBWALL, BWALL, PLAYER, ENEMY } EntityType;

typedef struct {
  WallType type;
  float health;
  Vector2 position;
} Wall;

typedef struct {
  Vector2 position;
  Vector2 velocity;
  Vector2 direction;
  std::vector<Vector2> shape;
} GameObject;

typedef struct {
  int id;
  Vector2 pos;
  Vector2 direction;
  bool is_shooting;
} Projectile;

#endif
