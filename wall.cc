#include "config.h"
#include "entities.h"
#include <raylib.h>
#include <vector>

int check_wall_collision(std::vector<Wall> walls, Vector2 position) {
  for (int i = 0; i < walls.size(); i++) {
    Wall wall = walls[i];
    Rectangle rect;

    rect.x = wall.position.x * CELL_SIZE;
    rect.y = wall.position.y * CELL_SIZE;

    rect.width = CELL_SIZE;
    rect.height = CELL_SIZE;

    // DEBUG:
    // DrawRectangleRec(rect, RED);

    if (CheckCollisionPointRec(position, rect)) {
      // DEBUG:
      // TraceLog(LOG_INFO, "WALL_COLLISION");
      return i;
    }
  }

  return -1;
}
