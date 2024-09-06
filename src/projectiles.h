#include <raylib.h>

#ifndef PROJECTILES_H
#define PROJECTILES_H

#define MAX_PROJECTILES 50

class Projectile {
public:
  Vector2 position;
  Vector2 direction;
  float angle;
  bool is_shooting;

  Projectile();
  Projectile(Vector2 position);
};

class ProjectilePool {
public:
  Projectile pool[MAX_PROJECTILES];

  ProjectilePool();

  int get_free_projectile();

  void allocate_projectile(int index);

  void deallocate_projectile(int index);
};

#endif
