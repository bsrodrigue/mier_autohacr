#include "wall.h"
#include "config.h"
#include "entities.h"
#include <raylib.h>

void draw_wall(Vector2 position, Texture2D texture) {
  DrawTexturePro(texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = CELL_OFFSET(position.x),
                  .y = CELL_OFFSET(position.y),
                  .width = CELL_SIZE,
                  .height = CELL_SIZE},
                 {0, 0}, 0, WHITE);
}

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
