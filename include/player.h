#pragma once

#include "entities.h"
#include "gate.h"
#include "item.h"
#include "projectiles.h"
#include "wall.h"
#include <raylib.h>

class Player {
public:
  Vector2 position;
  Vector2 direction; // Currently unused, could be repurposed
  Vector2 velocity;  // Now used for physics-based movement
  Texture2D texture;
  float angle;
  float speed;        // Now max_speed (renamed for clarity)
  float acceleration; // New: rate of speed increase
  float deceleration; // New: rate of speed decrease (friction)
  float health;
  float max_health;

  Player(Vector2 position = {0, 0});
  void handle_player_movement(std::vector<Vector2> wall_positions);
  void handle_player_movement_gates(Gate *gates); // Unchanged
  Vector2 get_confirmed_position(Vector2 next_position,
                                 std::vector<Vector2> wall_positions);
  Vector2 get_next_position(); // Will be updated for physics
  void load_texture(Texture2D texture);
  void draw();
};
