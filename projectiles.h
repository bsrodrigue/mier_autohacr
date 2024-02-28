#include <raylib.h>

#ifndef PROJECTILES_H
#define PROJECTILES_H

#define MAX_PROJECTILES 5

typedef struct {
  int id;
  Vector2 pos;
  Vector2 direction;
  bool is_shooting;
} Projectile;

void init_projectiles(Projectile *projectiles);

int get_free_projectile(Projectile *projectiles);

#endif
