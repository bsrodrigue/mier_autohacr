#include "player.h"
#include "collision.h"
#include "draw.h"
#include "entities.h"
#include <raylib.h>
#include <raymath.h>

#define PLAYER_HEALTH 10

Player::Player(Vector2 position) {
  this->position = position;
  this->velocity = {0, 0}; // Initialize velocity
  this->angle = 0;
  this->speed = 200.0f;        // Max speed (units per second)
  this->acceleration = 800.0f; // Normal acceleration
  this->deceleration = 400.0f; // Normal deceleration
  this->health = PLAYER_HEALTH;
  this->max_health = PLAYER_HEALTH;
  this->is_dashing = false;          // Not dashing by default
  this->dash_target = {0, 0};        // No target initially
  this->dash_acceleration = 1200.0f; // Higher acceleration for dash
  this->dash_max_speed = 400.0f;     // Higher max speed for dash
}

void Player::load_texture(Texture2D texture) { this->texture = texture; }

void Player::start_dash(Vector2 target) {
  is_dashing = true;
  dash_target = target;
  velocity = {0, 0}; // Reset velocity for a clean dash start
}

void Player::update_dash(std::vector<Vector2> wall_positions) {
  if (!is_dashing)
    return;

  float delta_time = GetFrameTime();
  Vector2 direction = Vector2Subtract(dash_target, position);
  float distance = Vector2Length(direction);

  // Smoothly reduce acceleration as we near the target
  float effective_acceleration = dash_acceleration * (distance / 50.0f);
  if (effective_acceleration < acceleration)
    effective_acceleration = acceleration;

  // Stop dashing if close enough to target
  if (distance < 5.0f) {
    is_dashing = false;
    velocity = Vector2Scale(velocity, 0.8f); // Smooth transition
    return;
  }

  // Normalize direction and apply scaled acceleration
  direction = Vector2Normalize(direction);
  velocity = Vector2Add(
      velocity, Vector2Scale(direction, effective_acceleration * delta_time));

  // Cap velocity during dash
  if (Vector2Length(velocity) > dash_max_speed) {
    velocity = Vector2Scale(Vector2Normalize(velocity), dash_max_speed);
  }

  // Calculate next position
  Vector2 next_position =
      Vector2Add(position, Vector2Scale(velocity, delta_time));

  // Check for collision and stop dash if hitting a wall
  int collision_index = check_cells_collision(wall_positions, next_position);
  if (collision_index != -1) {
    is_dashing = false;
    velocity = {0, 0}; // Stop velocity
    position = get_confirmed_position(next_position, wall_positions);
    return;
  }

  // No collision: update position
  position = next_position;
}

Vector2 Player::get_next_position() {
  Vector2 input_direction = {0, 0};
  if (!is_dashing) { // Normal movement only when not dashing
    if (IsKeyDown(KEY_W))
      input_direction.y -= 1.0f; // Up
    if (IsKeyDown(KEY_S))
      input_direction.y += 1.0f; // Down
    if (IsKeyDown(KEY_A))
      input_direction.x -= 1.0f; // Left
    if (IsKeyDown(KEY_D))
      input_direction.x += 1.0f; // Right

    // Normalize direction to prevent faster diagonal movement
    if (Vector2Length(input_direction) > 0.0f) {
      input_direction = Vector2Normalize(input_direction);
    }

    // Apply acceleration based on input
    float delta_time = GetFrameTime();
    if (Vector2Length(input_direction) > 0.0f) {
      velocity = Vector2Add(
          velocity, Vector2Scale(input_direction, acceleration * delta_time));
    }

    // Apply deceleration (friction) when no input
    if (Vector2Length(velocity) > 0.0f) {
      Vector2 friction =
          Vector2Scale(Vector2Normalize(velocity), -deceleration * delta_time);
      if (Vector2Length(velocity) < Vector2Length(friction)) {
        velocity = {0, 0}; // Stop if friction would overshoot
      } else {
        velocity = Vector2Add(velocity, friction);
      }
    }

    // Cap the velocity at max_speed
    if (Vector2Length(velocity) > speed) {
      velocity = Vector2Scale(Vector2Normalize(velocity), speed);
    }
  }

  return Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
}

Vector2 Player::get_confirmed_position(Vector2 next_position,
                                       std::vector<Vector2> wall_positions) {
  Vector2 confirmed_position = this->position;

  // Check Y movement (vertical)
  int collision_y =
      check_cells_collision(wall_positions, {position.x, next_position.y});
  if (collision_y == -1) {
    confirmed_position.y = next_position.y;
  } else {
    velocity.y = 0;
    Vector2 wall = wall_positions[collision_y];
    float overlap =
        fabs(next_position.y - wall.y) - 25.0f; // Assuming radius ~12.5f
    if (next_position.y < wall.y) {
      confirmed_position.y =
          wall.y - 25.0f - overlap; // Move out by overlap amount
    } else {
      confirmed_position.y = wall.y + 25.0f + overlap;
    }
  }

  // Check X movement (horizontal)
  int collision_x = check_cells_collision(
      wall_positions, {next_position.x, confirmed_position.y});
  if (collision_x == -1) {
    confirmed_position.x = next_position.x;
  } else {
    velocity.x = 0;
    Vector2 wall = wall_positions[collision_x];
    float overlap = fabs(next_position.x - wall.x) - 25.0f;
    if (next_position.x < wall.x) {
      confirmed_position.x =
          wall.x - 25.0f - overlap; // Move out by overlap amount
    } else {
      confirmed_position.x = wall.x + 25.0f + overlap;
    }
  }

  return confirmed_position;
}

void Player::handle_player_movement(std::vector<Vector2> wall_positions) {
  if (is_dashing) {
    update_dash(wall_positions);
    position = get_confirmed_position(position,
                                      wall_positions); // Double-check position
  } else {
    Vector2 next_position = get_next_position();
    position = get_confirmed_position(next_position, wall_positions);
  }
}

void Player::draw() { draw_ship(position, angle); }
