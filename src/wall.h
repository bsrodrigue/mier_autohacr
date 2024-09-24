#include "config.h"
#include "entities.h"
#include <raylib.h>
#include <vector>

#ifndef WALL_H
#define WALL_H

typedef enum { BREAKABLE, UNBREAKABLE } WallType;
typedef enum { SOLID, DESTROYED } WallState;

typedef struct {
  WallType type;
  WallState state;
  float health;
  Vector2 position;
} Wall;

int check_wall_collision(std::vector<Vector2> wall_positions, Vector2 position);
void draw_wall(Vector2 position, Texture2D texture);
Wall create_breakable_wall(Vector2 position);
Wall create_ubreakable_wall(Vector2 position);
#endif
