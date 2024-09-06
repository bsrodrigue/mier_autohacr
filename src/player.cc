#include "player.h"
#include "entities.h"
#include "wall.h"
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

void Player::handle_player_movement(std::vector<Wall> walls) {
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

  if (check_wall_collision(walls, {position.x, next_position.y}) == -1) {
    position.y = next_position.y;
  }
  if (check_wall_collision(walls, {next_position.x, position.y}) == -1) {
    position.x = next_position.x;
  }
}

void Player::draw() {
  DrawTexturePro(
      texture, {.x = 0, .y = 0, .width = 32, .height = 32},
      {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
      {16, 16}, angle + 90, WHITE);
}
