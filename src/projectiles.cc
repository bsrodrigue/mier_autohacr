
#include "projectiles.h"
#include "common.h"
#include <raylib.h>
#include <raymath.h>

Projectile::Projectile() {
  this->position = {0, 0};
  this->direction = {0, 0};
  this->is_shooting = false;
  this->angle = 0;
}

Projectile::Projectile(Vector2 position) {
  this->position = position;
  this->direction = position;
  this->is_shooting = false;
  this->angle = 0;
}

int ProjectilePool::get_free_projectile() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!this->pool[i].is_shooting) {
      return i;
    }
  }

  return -1;
}

ProjectilePool::ProjectilePool() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    this->pool[i] = Projectile();
  }
}

void ProjectilePool::allocate_projectile(int index) {
  this->pool[index].is_shooting = true;
}

void ProjectilePool::deallocate_projectile(int index) {
  pool[index].position = {0, 0};
  pool[index].is_shooting = false;
  pool[index].direction = {0, 0};
}
