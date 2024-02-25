#include "config.h"
#include "entities.h"
#include "game.h"
#include "level_editor.h"
#include "player.h"
#include "save.h"
#include "wall.h"
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <string.h>

Screen current_screen = GAME;

float velocity = 5;
float player_bullet_damage = 1;
float player_health = 5;

static int level_grid[CELL_COUNT][CELL_COUNT];

Camera2D camera;

std::vector<Wall> walls;

typedef enum {
  BASE,
} EnemyType;

typedef struct {
  float health;
  float shooting_interval;
  float last_shot;
  Vector2 position;
  float vision_radius;
  EnemyType type;
} Enemy;

Enemy create_enemy(Vector2 position, EnemyType type) {
  Enemy enemy;
  enemy.health = 3;
  enemy.position = position;
  enemy.shooting_interval = 1;
  enemy.last_shot = 0;
  enemy.vision_radius = 50 * 3;
  enemy.type = type;

  return enemy;
}

std::vector<Projectile> projectiles;
std::vector<Projectile> enemy_projectiles;
std::vector<Enemy> enemies;

void enemy_shoot(Enemy *enemy, Vector2 player_position) {
  Projectile projectile;
  projectile.pos = enemy->position;
  projectile.direction =
      Vector2Normalize(Vector2Subtract(player_position, projectile.pos));
  projectile.id = enemy_projectiles.size();
  enemy_projectiles.push_back(projectile);
}

void load_enemies() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_grid[y][x];
      if (type == BASE_ENEMY) {
        Enemy enemy =
            create_enemy({(float)CELL_OFFSET(x), (float)CELL_OFFSET(y)}, BASE);
        enemies.push_back(enemy);
      }
    }
  }
}

float half_len = 10;
float projectile_speed = 10.5;

GameObject player;

void define_shape(GameObject *game_obj) {
  // For the moment we will handle triangles only
  game_obj->shape.push_back(Vector2Add(game_obj->position, {0, -half_len}));
  game_obj->shape.push_back(
      Vector2Add(game_obj->position, {-half_len, half_len}));
  game_obj->shape.push_back(
      Vector2Add(game_obj->position, {half_len, half_len}));
}

void update_shape(GameObject *game_obj) {
  game_obj->shape[0] = Vector2Add(game_obj->position, {0, -half_len});
  game_obj->shape[1] = Vector2Add(game_obj->position, {-half_len, half_len});
  game_obj->shape[2] = Vector2Add(game_obj->position, {half_len, half_len});
}

void draw_game_obj(GameObject game_obj, Color color) {
  DrawTriangle(game_obj.shape[0], game_obj.shape[1], game_obj.shape[2], color);
}

void die(const char *message) {
  perror(message);
  exit(1);
}

void init() {
  InitWindow(WIN_WIDTH, WIN_HEIGHT, "NieR: AutoHacker");

  if (!IsWindowReady())
    die("Failed to initialize window\n");

  SetTargetFPS(FPS);

  ShowCursor();

  camera = {0};
  camera.target = player.position;
  camera.offset = {(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2};
  camera.rotation = 0;
  camera.zoom = 1.5;
}

void player_shoot(int pressed_key) {
  Vector2 mouse = get_world_mouse(camera);

  Projectile new_projectile;

  new_projectile.pos = player.position;
  new_projectile.direction =
      Vector2Normalize(Vector2Subtract(mouse, player.position));

  new_projectile.id = projectiles.size();

  projectiles.push_back(new_projectile);
}

void handle_single_press_input(int pressed_key) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    player_shoot(pressed_key);
}

void handle_game_input(int pressed_key) {
  handle_single_press_input(pressed_key);
  handle_player_movement(&player, velocity, walls);
}

void handle_global_input() {}

void handle_input(int pressed_key) {
  handle_global_input();

  switch (current_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    handle_game_input(pressed_key);
    break;
  case LEVEL_EDITOR:
    handle_level_input(&camera, pressed_key);
    break;
  }
}

void damage_wall(int index) {
  if ((walls[index].health - player_bullet_damage) <= 0) {
    walls.erase(walls.begin() + index);
  }

  walls.at(index).health = walls.at(index).health - player_bullet_damage;
}

void damage_player(float damage) {
  if ((player_health - damage) <= 0) {
    player_health = 0;
    // TODO: Implement Game Over (restart game);
    CloseWindow();
  }

  player_health -= damage;
}

void update_enemy_projectiles() {
  for (int i = 0; i < enemy_projectiles.size(); i++) {

    if (CheckCollisionCircles(enemy_projectiles[i].pos, 5, player.position,
                              10)) {
      damage_player(1);
    }

    // Recycle in object pool for optimal memory usage
    // Find better way to check many-to-many collisions
    int touched = check_wall_collision(walls, enemy_projectiles[i].pos);

    if (touched != -1) {
      switch (walls[touched].type) {
      case BREAKABLE:
        // It seems not safe to change the vector's size while looping
        // Should enemies be able to destroy walls?
        damage_wall(touched);
        break;
      case UNBREAKABLE:
        break;
      }

      enemy_projectiles.erase(enemy_projectiles.begin() + i);
      continue;
    }

    Vector2 projectile_pos =
        Vector2Add(enemy_projectiles[i].pos,
                   Vector2Multiply(enemy_projectiles[i].direction,
                                   {projectile_speed, projectile_speed}));
    enemy_projectiles[i].pos = projectile_pos;
  }
}

void update_player_projectiles() {
  for (int i = 0; i < projectiles.size(); i++) {

    // Recycle in object pool for optimal memory usage
    // Find better way to check many-to-many collisions
    int touched = check_wall_collision(walls, projectiles[i].pos);

    if (touched != -1) {
      switch (walls[touched].type) {
      case BREAKABLE:
        // It seems not safe to change the vector's size while looping
        damage_wall(touched);
        break;
      case UNBREAKABLE:
        break;
      }

      projectiles.erase(projectiles.begin() + i);
      continue;
    }

    Vector2 projectile_pos =
        Vector2Add(projectiles[i].pos,
                   Vector2Multiply(projectiles[i].direction,
                                   {projectile_speed, projectile_speed}));
    projectiles[i].pos = projectile_pos;
  }
}

void handle_enemy_shoot(Enemy *enemy) {
  float time = GetTime();

  if ((time - enemy->last_shot) >= enemy->shooting_interval) {
    enemy->last_shot = time;
    bool is_in_range = CheckCollisionPointCircle(
        player.position, enemy->position, enemy->vision_radius);

    if (is_in_range) {
      enemy_shoot(enemy, player.position);
    }
  }
}

void handle_enemy_behaviour() {
  for (int i = 0; i < enemies.size(); i++) {
    handle_enemy_shoot(&enemies[i]);

    bool is_in_range = CheckCollisionPointCircle(
        player.position, enemies[i].position, enemies[i].vision_radius);

    if (is_in_range) {
      Vector2 next_position =
          Vector2MoveTowards(enemies[i].position, player.position, 2);

      if (check_wall_collision(walls, next_position) == -1) {
        enemies[i].position = next_position;
      }

      DrawCircleV(enemies[i].position, enemies[i].vision_radius,
                  ColorAlpha(RED, 0.5));
    }
  }
}

void update_positions() {
  camera.target = player.position;
  handle_enemy_behaviour();
  update_enemy_projectiles();
  update_player_projectiles();
}

void handle_updates() {
  switch (current_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    update_positions();
    update_shape(&player);
    break;
  case LEVEL_EDITOR:
    break;
  }
}

void draw_projectiles() {
  for (int i = 0; i < projectiles.size(); i++) {
    DrawCircleV(projectiles[i].pos, 5, RED);
  }

  for (int i = 0; i < enemy_projectiles.size(); i++) {
    DrawCircleV(enemy_projectiles[i].pos, 5, LIGHTGRAY);
  }
}

void draw_enemies() {
  for (int i = 0; i < enemies.size(); i++) {
    DrawCircleV(enemies[i].position, 10, RED);
  }
}

void render_game() {
  draw_arena(walls);
  draw_projectiles();
  draw_enemies();
  draw_game_obj(player, WHITE);
}

void render_game_debug() {
  Vector2 mouse = get_world_mouse(camera);
  DrawLineV(player.position, mouse, RED);
}

const char *get_current_screen_title() {
  switch (current_screen) {
  case UNKNOWN:
    return "unkown";
  case MENU:
    return "menu";
  case GAME:
    return "game";
  case LEVEL_EDITOR:
    return "level-editor";
    break;
  default:
    return "unkown";
  }
}

void render_current_screen() {
  DrawText(get_current_screen_title(), TEXT_POS(1), TEXT_POS(1), 14, WHITE);
}

void render_global() { render_current_screen(); }

void render() {
  switch (current_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    render_game_debug();
    render_game();
    break;
  case LEVEL_EDITOR:
    render_level_editor(&camera);
    break;
  }

  render_global();
}

void select_screen(const char *screen_name) {
  if (strcmp(screen_name, "editor\n")) {
    load_level_editor();
    current_screen = LEVEL_EDITOR;
  }
}

void load_game() {
  load_level_file("level.hacc", level_grid);
  load_walls(walls, level_grid);
  load_enemies();
}

int main(int argc, char *argv[]) {
  if (argc == 2) {
    char *screen_name = argv[1];
    select_screen(screen_name);
  }

  init();

  load_game();

  Vector2 initial_player_pos = get_player_position(level_grid);

  player.position = initial_player_pos;

  define_shape(&player);

  while (!WindowShouldClose()) {
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(BLACK);
    int pressed_key = GetKeyPressed();

    handle_input(pressed_key);
    handle_updates();

    render();
    EndMode2D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
