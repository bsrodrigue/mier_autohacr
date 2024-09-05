#include "entities.h"
#include "projectiles.h"
#include "wall.h"
#include <raylib.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
  Vector2 position;
  Vector2 direction;
  Texture2D texture;
  ProjectilePool *projectiles;
  float angle;
  float speed;
  float health;
  float max_health;
  Player();
  Player(Vector2 position);
  void handle_player_movement(std::vector<Wall> walls);
  void load_texture(Texture2D texture);
  void draw();
  void shoot(Vector2 target);
  void subscribe_to_projectile_pool(ProjectilePool *pool);
};

#endif
