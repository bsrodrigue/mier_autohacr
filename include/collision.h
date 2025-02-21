#include "entities.h"
#include <raylib.h>
#include <vector>

#ifndef COLLISION_H
#define COLLISION_H

bool check_cell_collision(Vector2 cell_position, Vector2 position);
int check_cells_collision(std::vector<Vector2> cell_positions,
                          Vector2 position);
#endif
