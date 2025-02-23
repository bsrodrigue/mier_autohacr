#pragma once

#include "enemy.h"
#include "entities.h"
#include "item.h"
#include "wall.h"
#include <raylib.h>
#include <sys/types.h>
#include <variant>

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
