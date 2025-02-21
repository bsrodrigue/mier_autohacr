#include "shoot.h"

void shoot_target(Vector2 source, Vector2 target, ProjectilePool &pool) {
  int index = pool.get_free_projectile();

  if (index != -1) {
    pool.allocate_projectile(index);

    pool.pool[index].position = source;
    pool.pool[index].direction =
        Vector2Normalize(Vector2Subtract(target, source));

    pool.pool[index].angle = get_angle_relative_to(target, source);
  }
}

void shoot_straight(Vector2 source, Vector2 direction, ProjectilePool &pool) {
  int index = pool.get_free_projectile();

  if (index != -1) {
    pool.allocate_projectile(index);

    pool.pool[index].position = source;
    pool.pool[index].direction = direction;

    float angle_rad = atan2f(direction.y, direction.x);

    pool.pool[index].angle = angle_rad * RAD2DEG;
  }
}
