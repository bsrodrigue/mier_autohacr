#include "collision.h"
#include "config.h"

bool check_cell_collision(Vector2 cell_position, Vector2 position) {
  Rectangle rect;

  rect.x = cell_position.x;
  rect.y = cell_position.y;

  rect.width = CELL_SIZE;
  rect.height = CELL_SIZE;

  return CheckCollisionPointRec(position, rect);
}

int check_cells_collision(std::vector<Vector2> cell_positions,
                          Vector2 position) {

  for (int i = 0; i < cell_positions.size(); i++) {

    if (check_cell_collision(cell_positions[i], position)) {
      return i;
    }
  }

  return -1;
}
