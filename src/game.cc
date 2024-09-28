#include "common.h"
#include "draw.h"
#include "entities.h"
#include "level_editor.h"
#include "wall.h"
#include <raylib.h>
#include <vector>

void draw_arena(std::vector<Wall> &walls) {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 position = walls[i].position;
    draw_wall(position, walls[i].type);
  }
}

Vector2 get_player_position_for_game(EditorGridCell (*grid)[100]) {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      EditorGridCell cell = grid[y][x];

      if (std::holds_alternative<EditorPlayer>(cell.entity)) {
        return {(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)};
      }
    }
  }

  return {-1, -1};
}
