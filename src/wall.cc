#include "wall.h"
#include "collision.h"
#include "config.h"
#include "entities.h"
#include <raylib.h>

Wall create_breakable_wall(Vector2 position) {
  Wall wall;
  wall.type = BREAKABLE;
  wall.health = BREAKABLE_WALL_HEALTH;
  wall.position = position;

  return wall;
}

Wall create_ubreakable_wall(Vector2 position) {
  Wall wall;
  wall.type = UNBREAKABLE;
  wall.health = 0;
  wall.position = position;

  return wall;
}

void draw_wall(Vector2 position, Texture2D texture) {
  DrawTexturePro(texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = (position.x),
                  .y = (position.y),
                  .width = CELL_SIZE,
                  .height = CELL_SIZE},
                 {0, 0}, 0, WHITE);
}

// Just a wrapper around check_cells_collision
int check_wall_collision(std::vector<Vector2> wall_positions,
                         Vector2 position) {
  return check_cells_collision(wall_positions, position);
}
