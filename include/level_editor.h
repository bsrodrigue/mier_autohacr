#pragma once

#include "config.h"
#include "enemy.h"
#include "entities.h"
#include "item.h"
#include "wall.h"
#include <raylib.h>
#include <sys/types.h>
#include <variant>

constexpr int TYPE_COUNT = 8;
constexpr float ENTITY_DROPDOWN_HEIGHT = 25;
constexpr float ENTITY_DROPDOWN_WIDTH = 300;

// Forward Declarations
struct EditorPlayer;
struct EditorWall;
struct EditorVoid;
struct EditorEnemy;
struct EditorWarpzone;
struct EditorItem;
struct EditorGate;

using ItemParams = EditorItem;

class EntityVisitor {
public:
  virtual void visit(const EditorVoid &entity) = 0;
  virtual void visit(const EditorWall &entity) = 0;
  virtual void visit(const EditorEnemy &entity) = 0;
  virtual void visit(const EditorWarpzone &entity) = 0;
  virtual void visit(const EditorPlayer &entity) = 0;
  virtual void visit(const EditorItem &entity) = 0;
  virtual void visit(const EditorGate &entity) = 0;

  virtual ~EntityVisitor() = default;
};

class Entity {
public:
  virtual void accept(EntityVisitor &visitor) const = 0;
  virtual ~Entity() = default;
};

struct EditorVoid : public Entity {
  EditorVoid() {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorWarpzone : public Entity {
  Vector2 destination;

  EditorWarpzone(Vector2 dest) : destination(dest) {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorItem : public Entity {
  ItemEffect effect;
  ItemUsage usage;

  EditorItem(ItemEffect e, ItemUsage u) : effect(e), usage(u) {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorWall : public Entity {
  WallType type;

  EditorWall(WallType t) : type(t) {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorEnemy : public Entity {
  EnemyType type = BASE_ENEMY;
  float enemy_health = 0;
  float shooting_interval = 0;

  bool tracks_player = false;

  EditorEnemy() {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorGate : public Entity {
  EditorGate() {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorPlayer : public Entity {
  EditorPlayer() {}

  void accept(EntityVisitor &visitor) const override { visitor.visit(*this); }
};

struct EditorGridCell {
  EntityType type;
  std::variant<EditorVoid, EditorWall, EditorEnemy, EditorWarpzone,
               EditorPlayer, EditorItem, EditorGate>
      entity;
};

class LevelEditor {
public:
  const char *filename;

  // ------------------------[ Editor State ]------------------------------ //
  // General
  bool can_change_entity = true;
  bool placing_mode = false;

  // Inspection
  EditorGridCell *hovered_cell = nullptr;
  EditorGridCell *inspected_cell = nullptr;
  int inspected_cell_row = 0;
  int inspected_cell_col = 0;

  // Entity Dropdown
  bool entity_dropdown_is_open = false;
  EntityType current_entity = BWALL_ENTITY;

  // ----[Entities]
  // Enemies
  float enemy_health = 0;

  // Warpzone
  bool is_placing_warpzone_origin = true;
  Vector2 warpzone_origin_pos = {-1, -1};

  // ----[Items]
  ItemParams item_params{ItemEffect::NO_EFFECT, ItemUsage::NO_USAGE};

  // ----------------------------[ Grid ]---------------------------------- //
  EditorGridCell grid[CELL_COUNT][CELL_COUNT];

  LevelEditor() {}

  void init_editor();
  void load_level_data();
  void create_level_data();
  void save_level();

  Vector2 get_player_position();

  void handle_player_position_clearing();
  void place_entity(Vector2 mouse);
  void handle_entity_placement(Vector2 mouse, EntityType type);
};

void render_level_editor(Camera2D *camera);
void render_level_editor_ui(Camera2D *camera);
void handle_editor_input(Camera2D *camera, int pressed_key);
void load_level_editor(const char *filename);
