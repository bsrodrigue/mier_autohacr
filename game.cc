#include "common.h"
#include "config.h"
#include "entities.h"
#include <raylib.h>
#include <vector>

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

void generate_walls(std::vector<Wall> &walls) {
  for (float i = 0; i < CELL_COUNT; i++) {
    Wall wall_up = create_ubreakable_wall({i, 0});
    Wall wall_down = create_ubreakable_wall({i, CELL_COUNT - 1});
    walls.push_back(wall_up);
    walls.push_back(wall_down);
  }

  for (float i = 0; i < CELL_COUNT; i++) {
    Wall wall_up = create_ubreakable_wall({0, i});
    Wall wall_down = create_ubreakable_wall({CELL_COUNT - 1, i});
    walls.push_back(wall_up);
    walls.push_back(wall_down);
  }
}

void load_walls(std::vector<Wall> &walls,
                int level_grid[CELL_COUNT][CELL_COUNT]) {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_grid[y][x];
      if (type == UBWALL) {
        walls.push_back(create_ubreakable_wall({(float)x, (float)y}));
      }

      else if (type == BWALL) {
        walls.push_back(create_breakable_wall({(float)x, (float)y}));
      }
    }
  }
}

void draw_arena(std::vector<Wall> &walls) {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 position = walls[i].position;
    draw_cell(position.x, position.y, walls[i].type == BREAKABLE ? RED : WHITE);
  }
}
