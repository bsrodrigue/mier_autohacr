#include "config.h"
#include "entities.h"
#include "level_editor.h"
#include "wall.h"
#include <raylib.h>

#ifndef GAME_H
#define GAME_H

void draw_arena(std::vector<Wall> &walls, Texture2D ubwall_texture,
                Texture2D bwall_texture);

Wall create_breakable_wall(Vector2 position);

Wall create_ubreakable_wall(Vector2 position);

void generate_walls(std::vector<Wall> &walls);
#endif
