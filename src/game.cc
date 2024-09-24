#include "common.h"
#include "entities.h"
#include "level_editor.h"
#include "wall.h"
#include <raylib.h>
#include <vector>

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
