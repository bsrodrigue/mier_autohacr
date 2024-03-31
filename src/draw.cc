#include "textures.h"
#include "wall.h"

void draw_ubwall(Vector2 position) { draw_wall(position, ubwall_texture); }
void draw_bwall(Vector2 position) { draw_wall(position, bwall_texture); }

void draw_warpzone(Vector2 position) {
  DrawTexturePro(warzone_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = (position.x * CELL_SIZE),
                  .y = (position.y * CELL_SIZE),
                  .width = 25,
                  .height = 25},
                 {0, 0}, 0, WHITE);
}

void draw_ship(Vector2 position) {
  DrawTexturePro(player_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = (position.x * CELL_SIZE),
                  .y = (position.y * CELL_SIZE),
                  .width = 25,
                  .height = 25},
                 {0, 0}, 0, WHITE);
}

void draw_entity(EntityType type, Vector2 position) {
  // TODO: This does not belong here
  draw_wall(position, floor_texture);
  switch (type) {
  case EMPTY:
    break;
  case UBWALL:
    draw_wall(position, ubwall_texture);
    break;
  case BWALL:
    draw_wall(position, bwall_texture);
    break;
  case PLAYER:
    draw_ship(position);
    break;
  case BASE_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(RED, 1));
    break;
  case SENTRY_A_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(PURPLE, 1));
    break;
  case WARPZONE:
    draw_warpzone(position);
    break;
  }
}
