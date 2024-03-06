
#include "projectiles.h"

void init_projectiles(Projectile *projectiles) {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    Projectile p = projectiles[i];
    p.pos = {0, 0};
    p.direction = {0, 0};
    p.is_shooting = false;
  }
}

int get_free_projectile(Projectile *projectiles) {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!projectiles[i].is_shooting)
      return i;
  }

  return -1;
}
