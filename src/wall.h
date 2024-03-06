#include "config.h"
#include "entities.h"
#include <raylib.h>
#include <vector>

#ifndef WALL_H
#define WALL_H
int check_wall_collision(std::vector<Wall> walls, Vector2 position);
void draw_wall(Vector2 position, Texture2D texture);
#endif
