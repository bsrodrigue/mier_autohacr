#include "config.h"
#include "entities.h"
#include <raylib.h>

#ifndef GAME_H
#define GAME_H
void load_walls(std::vector<Wall> &walls,
                int level_grid[CELL_COUNT][CELL_COUNT]);
void draw_arena(std::vector<Wall> &walls, Texture2D texture);

Wall create_breakable_wall(Vector2 position);

Wall create_ubreakable_wall(Vector2 position);

void generate_walls(std::vector<Wall> &walls);
#endif
