#include "player.h"
#include "entities.h"
#include "wall.h"
#include <raylib.h>
#include <raymath.h>

Player::Player() {
  this->position = {0, 0};
  this->direction = {0, 0};
  this->angle = 0;
  this->speed = 8;
  this->health = 20;
  this->projectiles = NULL;
}

Player::Player(Vector2 position) {
  this->position = position;
  this->direction = position;
  this->angle = 0;
  this->speed = 8;
  this->health = 20;
  this->projectiles = NULL;
}

void Player::load_texture(Texture2D texture) { this->texture = texture; }

void Player::handle_player_movement(std::vector<Wall> walls) {
  Vector2 vertical_movement = {0, 0};
  Vector2 horizontal_movement = {0, 0};

  bool can_move_vertical = true;
  bool can_move_horizontal = true;

  if (IsKeyDown(KEY_W)) {
    vertical_movement = {0, -speed};
  } else if (IsKeyDown(KEY_S)) {
    vertical_movement = {0, speed};
  }

  if (IsKeyDown(KEY_A)) {
    horizontal_movement = {-speed, 0};
  } else if (IsKeyDown(KEY_D)) {
    horizontal_movement = {speed, 0};
  }

  Vector2 next_vertical_position = Vector2Add(position, vertical_movement);

  if (check_wall_collision(walls, next_vertical_position) != -1) {
    can_move_vertical = false;
  }

  Vector2 next_horizontal_position = Vector2Add(position, horizontal_movement);

  if (check_wall_collision(walls, next_horizontal_position) != -1) {
    can_move_horizontal = false;
  }

  if (can_move_vertical) {
    position = Vector2Add(position, vertical_movement);
  }

  if (can_move_horizontal) {
    position = Vector2Add(position, horizontal_movement);
  }
}

void Player::draw() {
  DrawTexturePro(
      texture, {.x = 0, .y = 0, .width = 32, .height = 32},
      {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
      {16, 16}, angle + 90, WHITE);
}

void Player::subscribe_to_projectile_pool(ProjectilePool *pool) {
  this->projectiles = pool;
}

void Player::shoot(Vector2 target) {
  if (this->projectiles == NULL)
    return;

  int result = this->projectiles->get_free_projectile();
  if (result != -1) {
    this->projectiles->allocate_projectile(result, this->position, target);
  }
}
