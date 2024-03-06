#include "entities.h"
#include "wall.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

void handle_player_movement(GameObject *player, float velocity,
                            std::vector<Wall> walls) {
  Vector2 vertical_movement = {0, 0};
  Vector2 horizontal_movement = {0, 0};

  bool can_move_vertical = true;
  bool can_move_horizontal = true;

  if (IsKeyDown(KEY_UP)) {
    vertical_movement = {0, -velocity};
  } else if (IsKeyDown(KEY_DOWN)) {
    vertical_movement = {0, velocity};
  }

  if (IsKeyDown(KEY_LEFT)) {
    horizontal_movement = {-velocity, 0};
  } else if (IsKeyDown(KEY_RIGHT)) {
    horizontal_movement = {velocity, 0};
  }

  Vector2 next_vertical_position =
      Vector2Add(player->position, vertical_movement);

  if (check_wall_collision(walls, next_vertical_position) != -1) {
    can_move_vertical = false;
  }

  Vector2 next_horizontal_position =
      Vector2Add(player->position, horizontal_movement);

  if (check_wall_collision(walls, next_horizontal_position) != -1) {
    can_move_horizontal = false;
  }

  if (can_move_vertical) {
    player->position = Vector2Add(player->position, vertical_movement);
  }

  if (can_move_horizontal) {
    player->position = Vector2Add(player->position, horizontal_movement);
  }
}
