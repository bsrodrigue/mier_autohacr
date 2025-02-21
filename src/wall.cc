#include "wall.h"
#include "collision.h"
#include "config.h"
#include "entities.h"
#include <raylib.h>

Wall create_breakable_wall(Vector2 position) {
  Wall wall;
  wall.type = BREAKABLE_WALL;
  wall.health = BREAKABLE_WALL_HEALTH;
  wall.position = position;

  return wall;
}

Wall create_ubreakable_wall(Vector2 position) {
  Wall wall;
  wall.type = UNBREAKABLE_WALL;
  wall.health = 0;
  wall.position = position;

  return wall;
}

// Just a wrapper around check_cells_collision
int check_wall_collision(std::vector<Vector2> wall_positions,
                         Vector2 position) {
  return check_cells_collision(wall_positions, position);
}
