#include "player.h"
#include "collision.h"
#include "draw.h"
#include "entities.h"
#include <raylib.h>
#include <raymath.h>

#define PLAYER_HEALTH 20

Player::Player() {
  this->position = {0, 0};
  this->direction = {0, 0};
  this->angle = 0;
  this->speed = 3;
  this->health = PLAYER_HEALTH;
  this->max_health = PLAYER_HEALTH;
}

Player::Player(Vector2 position) {
  this->position = position;
  this->direction = position;
  this->angle = 0;
  this->speed = 3;
  this->health = PLAYER_HEALTH;
  this->max_health = PLAYER_HEALTH;
}

void Player::load_texture(Texture2D texture) { this->texture = texture; }

Vector2 Player::get_next_position() {
  Vector2 movement = {0, 0};

  if (IsKeyDown(KEY_W))
    movement.y = -speed;
  if (IsKeyDown(KEY_S))
    movement.y = speed;
  if (IsKeyDown(KEY_A))
    movement.x = -speed;
  if (IsKeyDown(KEY_D))
    movement.x = speed;

  Vector2 next_position = Vector2Add(position, movement);

  return next_position;
}

Vector2 Player::get_confirmed_position(Vector2 next_position,
                                       std::vector<Vector2> cell_positions) {

  Vector2 confirmed_position = this->position;
  if (check_cells_collision(cell_positions, {position.x, next_position.y}) ==
      -1) {
    confirmed_position.y = next_position.y;
  }
  if (check_cells_collision(cell_positions, {next_position.x, position.y}) ==
      -1) {
    confirmed_position.x = next_position.x;
  }

  return confirmed_position;
}

void Player::handle_player_movement(std::vector<Vector2> wall_positions) {
  Vector2 next_position = get_next_position();
  this->position = get_confirmed_position(next_position, wall_positions);
}

void Player::draw() { draw_ship(position, angle + 90); }
