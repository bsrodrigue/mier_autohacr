#include "enemy.h"
#include "entities.h"
#include "raylib.h"
#include "wall.h"

#ifndef DRAW_H
#define DRAW_H

#define GAME_TEXTURE_SIZE 32

void draw_ubwall(Vector2 position);
void draw_bwall(Vector2 position);
void draw_floor(Vector2 position);
void draw_wall(Vector2 position, WallType type);
void draw_texture_cell(Vector2 position, Texture2D texture, float rotation);

void draw_game_texture(Vector2 position, Texture2D texture, float rotation = 0,
                       float width = GAME_TEXTURE_SIZE,
                       float height = GAME_TEXTURE_SIZE,
                       Vector2 origin = {(GAME_TEXTURE_SIZE / 2.0f),
                                         (GAME_TEXTURE_SIZE / 2.0f)});

void draw_warpzone(Vector2 position);
void draw_ship(Vector2 position, float rotation);
void draw_healing_chip(Vector2 position, float rotation);
void draw_base_enemy(Vector2 position, float rotation);
void draw_target_cursor(Vector2 position, float rotation);
void draw_projectile(Vector2 position, float rotation);
void draw_editor_entity(EntityType type, Vector2 position);
void render_mouse_hover_grid(Vector2 mouse, EntityType type);

#endif
