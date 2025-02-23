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
  Vector2 velocity;  // Used for physics-based movement
  Texture2D texture;
  float angle;
  float speed;        // Max speed
  float acceleration; // Rate of speed increase
  float deceleration; // Rate of speed decrease (friction)
  float health;
  float max_health;

  // Dash-related members
  bool is_dashing;         // Is the player currently dashing?
  Vector2 dash_target;     // Target position for the dash
  float dash_acceleration; // Higher acceleration for dashing
  float dash_max_speed;    // Higher max speed during dash

  Player(Vector2 position = {0, 0});
  void handle_player_movement(std::vector<Vector2> wall_positions);
  void handle_player_movement_gates(Gate *gates);
  Vector2 get_confirmed_position(Vector2 next_position,
                                 std::vector<Vector2> wall_positions);
  Vector2 get_next_position();
  void load_texture(Texture2D texture);
  void draw();
  void start_dash(Vector2 target);
  void update_dash(
      std::vector<Vector2> wall_positions); // Updated: Pass wall_positions
};
