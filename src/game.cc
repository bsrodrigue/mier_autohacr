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
