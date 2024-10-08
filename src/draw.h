#include "entities.h"
#include "raylib.h"

#ifndef DRAW_H
#define DRAW_H

void draw_ubwall(Vector2 position);
void draw_bwall(Vector2 position);
void draw_warpzone(Vector2 position);
void draw_ship(Vector2 position);
void draw_editor_entity(EntityType type, Vector2 position);
void render_mouse_hover_grid(Vector2 mouse, EntityType type);

#endif
