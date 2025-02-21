#include "entities.h"
#include "gate.h"
#include "item.h"
#include "projectiles.h"
#include "wall.h"
#include <raylib.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
  Vector2 position;
  Vector2 direction;
  Vector2 velocity;
  Texture2D texture;
  float angle;
  float speed;
  float health;
  float max_health;
  Player();
  Player(Vector2 position);
  void handle_player_movement(std::vector<Vector2> wall_positions);
  void handle_player_movement_gates(Gate *gates);
  Vector2 get_confirmed_position(Vector2 next_position,
                                 std::vector<Vector2> wall_positions);
  Vector2 get_next_position();
  void load_texture(Texture2D texture);
  void draw();
};

#endif
