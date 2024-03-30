#include "common.h"
#include "config.h"
#include "entities.h"
#include "level_editor.h"
#include "wall.h"
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

void load_walls(std::vector<Wall> &walls,
                EditorGridCell level_grid[CELL_COUNT][CELL_COUNT]) {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_grid[y][x].type;
      if (type == UBWALL) {
        walls.push_back(create_ubreakable_wall({(float)x, (float)y}));
      }

      else if (type == BWALL) {
        walls.push_back(create_breakable_wall({(float)x, (float)y}));
      }
    }
  }
}

void draw_arena(std::vector<Wall> &walls, Texture2D ubwall_texture,
                Texture2D bwall_texture) {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 position = walls[i].position;
    switch (walls[i].type) {
    case BREAKABLE:
      draw_wall(position, bwall_texture);
      break;
    case UNBREAKABLE:
      draw_wall(position, ubwall_texture);
      break;
    }
  }
}
