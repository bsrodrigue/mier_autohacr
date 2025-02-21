#pragma once

#include "common.h"
#include "projectiles.h"
#include <raylib.h>
#include <raymath.h>

void shoot_target(Vector2 source, Vector2 target, ProjectilePool &pool);
void shoot_straight(Vector2 source, Vector2 direction, ProjectilePool &pool);
