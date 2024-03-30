#include "common.h"
#include "config.h"
#include "enemy.h"
#include "entities.h"
#include "game.h"
#include "level.h"
#include "level_editor.h"
#include "player.h"
#include "projectiles.h"
#include "save.h"
#include "screen.h"
#include "textures.h"
#include "wall.h"
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <string.h>

Texture2D player_texture;
Texture2D ubwall_texture;
Texture2D bwall_texture;
Texture2D floor_texture;
Texture2D target_texture;
Texture2D sentinel_texture;
Texture2D sentinel_head_texture;
Texture2D sentinel_barrel_texture;
Texture2D warzone_texture;
Texture2D projectile_texture;

ScreenManager screen_manager;

Level level;

char *level_file;

float player_bullet_damage = 1;
float projectile_speed = 8;

static Camera2D camera;

static std::vector<Wall> walls;

static int enemy_count = 0;
Enemy enemies[MAX_ENEMIES];

ProjectilePool player_projectiles;
ProjectilePool enemy_projectiles;

Player player;

void enemy_shoot(Enemy enemy, Vector2 target) {
  int result = enemy_projectiles.get_free_projectile();
  if (result != -1) {
    enemy_projectiles.allocate_projectile(result, enemy.position, target);
  }
}

void enemy_shoot_player(Enemy enemy, Vector2 player_position) {
  enemy_shoot(enemy, player_position);
}

void enemy_shoot_star(Enemy enemy) {
  enemy_shoot(enemy, Vector2Add(enemy.position, {0, -1}));
  enemy_shoot(enemy, Vector2Add(enemy.position, {1, 0}));
  enemy_shoot(enemy, Vector2Add(enemy.position, {0, 1}));
  enemy_shoot(enemy, Vector2Add(enemy.position, {-1, 0}));
}

void player_shoot() {
  Vector2 mouse = get_world_mouse(camera);
  player.shoot(mouse);
}

void load_enemies() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level.grid[y][x];
      if (type == BASE_ENEMY) {
        Enemy enemy =
            create_enemy({(float)CELL_OFFSET(x), (float)CELL_OFFSET(y)}, BASE);
        enemies[enemy_count++] = enemy;
      }

      else if (type == SENTRY_A_ENEMY) {
        Enemy enemy = create_enemy(
            {(float)CELL_OFFSET(x), (float)CELL_OFFSET(y)}, SENTRY_A);
        enemies[enemy_count++] = enemy;
      }
    }
  }
}

void draw_game_texture(Vector2 position, float angle, Texture2D texture) {
  DrawTexturePro(
      texture, {.x = 0, .y = 0, .width = 32, .height = 32},
      {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
      {16, 16}, angle, WHITE);
}

void die(const char *message) {
  perror(message);
  exit(1);
}

void init_game_camera() {
  camera.target = player.position;
  camera.offset = {(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2};
  camera.rotation = 0;
  camera.zoom = 2;
}

void init_window() {
  InitWindow(WIN_WIDTH, WIN_HEIGHT, "Mia: Autohacker");

  if (!IsWindowReady())
    die("Failed to initialize window\n");

  SetTargetFPS(FPS);

  HideCursor();
}

void handle_single_press_input() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    player_shoot();
}

void handle_game_input() {
  handle_single_press_input();
  player.handle_player_movement(walls);
}

void handle_input(int pressed_key) {
  switch (screen_manager.active_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    handle_game_input();
    break;
  case LEVEL_EDITOR:
    handle_editor_input(&camera, pressed_key);
    break;
  }
}

void damage_enemy(int index) {
  if ((enemies[index].health - player_bullet_damage) <= 0) {
    enemies[index].state = DEAD;
  }

  enemies[index].health = enemies[index].health - player_bullet_damage;
}

void damage_wall(int index) {
  if ((walls[index].health - player_bullet_damage) <= 0) {
    walls.erase(walls.begin() + index);
  }

  walls.at(index).health = walls.at(index).health - player_bullet_damage;
}

void damage_player(float damage) {
  if ((player.health - damage) <= 0) {
    player.health = 0;
    // TODO: Implement Game Over (restart game);
    CloseWindow();
  }

  player.health -= damage;
}

int check_enemy_collision(Vector2 position, float radius) {
  for (int i = 0; i < enemy_count; i++) {
    if (enemies[i].state == DEAD)
      continue;
    if (CheckCollisionCircles(position, radius, enemies[i].position, 10))
      return i;
  }

  return -1;
}

void update_enemy_projectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!enemy_projectiles.pool[i].is_shooting)
      continue;

    if (CheckCollisionCircles(enemy_projectiles.pool[i].position, 5,
                              player.position, 10)) {
      damage_player(1);
    }

    // Find better way to check many-to-many collisions
    int touched =
        check_wall_collision(walls, enemy_projectiles.pool[i].position);

    if (touched != -1) {
      switch (walls[touched].type) {
      case BREAKABLE:
        // Should enemies be able to destroy walls?
        damage_wall(touched);
        break;
      case UNBREAKABLE:
        break;
      }

      enemy_projectiles.pool[i].is_shooting = false;
      continue;
    }

    Vector2 projectile_pos =
        Vector2Add(enemy_projectiles.pool[i].position,
                   Vector2Multiply(enemy_projectiles.pool[i].direction,
                                   {projectile_speed, projectile_speed}));
    enemy_projectiles.pool[i].position = projectile_pos;
  }
}

void reset_projectile(int index, Projectile *projectiles) {
  projectiles[index].is_shooting = false;
  projectiles[index].position = {0, 0};
  projectiles[index].direction = {0, 0};
}

void update_player_projectiles() {
  // TODO: Do you seriously want to loop over all the projectiles?
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!player_projectiles.pool[i].is_shooting)
      continue;

    int enemy = check_enemy_collision(player_projectiles.pool[i].position, 5);

    if (enemy != -1) {
      damage_enemy(enemy);
      player_projectiles.deallocate_projectile(i);
      continue;
    }

    // Recycle in object pool for optimal memory usage
    // Find better way to check many-to-many collisions
    int touched =
        check_wall_collision(walls, player_projectiles.pool[i].position);

    if (touched != -1) {
      switch (walls[touched].type) {
      case BREAKABLE:
        // It seems not safe to change the vector's size while looping
        damage_wall(touched);
        break;
      case UNBREAKABLE:
        break;
      }
      player_projectiles.deallocate_projectile(i);
      continue;
    }

    Vector2 projectile_pos =
        Vector2Add(player_projectiles.pool[i].position,
                   Vector2Multiply(player_projectiles.pool[i].direction,
                                   {projectile_speed, projectile_speed}));
    player_projectiles.pool[i].position = projectile_pos;
  }
}

void handle_enemy_shoot(Enemy *enemy, bool is_in_range) {
  float time = GetTime();

  if ((time - enemy->last_shot) >= enemy->shooting_interval) {
    enemy->last_shot = time;

    if (is_in_range) {
      switch (enemy->type) {
      case BASE:
        enemy_shoot_player(*enemy, player.position);
        break;
      case SENTRY_A:
        enemy_shoot_star(*enemy);
        break;
      }
    }
  }
}

void handle_enemy_behaviour() {
  for (int i = 0; i < enemy_count; i++) {
    if (enemies[i].state == DEAD)
      continue;

    bool is_in_range = CheckCollisionPointCircle(
        player.position, enemies[i].position, enemies[i].vision_radius);

    handle_enemy_shoot(&enemies[i], is_in_range);

    if (is_in_range) {

      switch (enemies[i].type) {
      case BASE: {
        enemies[i].shooting_angle =
            get_angle_relative_to(player.position, enemies[i].position);
        Vector2 next_position =
            Vector2MoveTowards(enemies[i].position, player.position, 2);

        if (check_wall_collision(walls, next_position) == -1) {
          enemies[i].position = next_position;
        }
      } break;
      case SENTRY_A:
        break;
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

void update_player_angle() {
  Vector2 mouse = get_world_mouse(camera);
  player.angle = get_angle_relative_to(mouse, player.position);
}

void handle_updates() {
  switch (screen_manager.active_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    update_positions();
    update_player_angle();
    break;
  case LEVEL_EDITOR:
    break;
  }
}

void draw_player_projectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (player_projectiles.pool[i].is_shooting) {
      draw_game_texture(player_projectiles.pool[i].position,
                        player_projectiles.pool[i].angle + 90,
                        projectile_texture);
    }
  }
}

void draw_enemy_projectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (enemy_projectiles.pool[i].is_shooting) {

      draw_game_texture(enemy_projectiles.pool[i].position,
                        enemy_projectiles.pool[i].angle + 90,
                        projectile_texture);
    }
  }
}

void draw_projectiles() {
  draw_enemy_projectiles();
  draw_player_projectiles();
}

void draw_enemies() {
  for (int i = 0; i < enemy_count; i++) {
    if (enemies[i].state == DEAD)
      continue;

    switch (enemies[i].type) {
    case BASE: {
      Vector2 position = enemies[i].position;
      DrawTexturePro(
          sentinel_barrel_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
          {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
          {16, 32}, enemies[i].shooting_angle + 90, WHITE);

      draw_game_texture(enemies[i].position, enemies[i].shooting_angle + 90,
                        sentinel_head_texture);
    } break;
    case SENTRY_A:
      Vector2 position = enemies[i].position;
      DrawTexturePro(
          sentinel_texture, {.x = 0, .y = 0, .width = 64, .height = 64},
          {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
          {16, 16}, 0, WHITE);
      break;
    }
  }
}

void draw_floor() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      draw_wall({(float)x, (float)y}, floor_texture);
    }
  }
}

void draw_player_target() {
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  DrawTexturePro(
      target_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
      {.x = (position.x), .y = (position.y), .width = 32, .height = 32},
      {16, 16}, player.angle + 90, WHITE);
}

void render_game() {
  draw_floor();
  draw_arena(walls, ubwall_texture, bwall_texture);
  draw_projectiles();
  draw_enemies();
  player.draw();
  draw_player_target();
}

const char *get_current_screen_title() {
  switch (screen_manager.active_screen) {
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
  switch (screen_manager.active_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    render_game();
    break;
  case LEVEL_EDITOR:
    render_level_editor(&camera);
    break;
  }

  render_global();
}

// TODO: Optimize level loading
void load_level() {
  load_walls(walls, level.grid);
  load_enemies();
}

void init_player() {
  player.subscribe_to_projectile_pool(&player_projectiles);
  player.load_texture(player_texture);
  // player.position = get_player_position(level.grid);
  player.position = {0, 0};
}

void ScreenManager::init_game_screen() {
  level.filename = level_file;
  level.load_level_data();
  init_player();
  load_level();
  init_game_camera();
}

void ScreenManager::init_level_editor_screen(const char *filename) {
  ShowCursor();
  camera.offset = {(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2};
  camera.rotation = 0;
  camera.zoom = 1;
  load_level_editor(filename);
}

void ScreenManager::handle_screen_change() {
  if (!this->screen_changed())
    return;

  this->previous_screen = this->active_screen;

  switch (this->active_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    this->init_game_screen();
    break;
  case LEVEL_EDITOR:
    this->init_level_editor_screen(level_file);
    break;
  }
}

void set_initial_screen(const char *game_mode) {
  if (strcmp("editor", game_mode) == 0) {
    screen_manager.set_screen(LEVEL_EDITOR);
  }

  else if (strcmp("game", game_mode) == 0) {
    screen_manager.set_screen(GAME);
  }
}

// Why is game development so hard?
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: autohacka [gamemode] [level_file]\n");
    return 1;
  }

  init_window();
  load_textures();

  char *game_mode = argv[1];
  level_file = argv[2];

  set_initial_screen(game_mode);

  while (!WindowShouldClose()) {
    screen_manager.handle_screen_change();
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);

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
