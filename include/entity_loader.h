#pragma once

#include "gate.h"
#include "item.h"
#include "level_editor.h"
#include "warpzone.h"
#include <raylib.h>
#include <vector>

class EntityLoader : public EntityVisitor {
public:
  Vector2 position;

  EntityLoader(Vector2 position, std::vector<Wall> &walls,
               std::vector<Vector2> &wall_positions, std::vector<Gate> &gates,
               std::vector<Vector2> &gate_positions, Enemy *enemies,
               int &enemy_count, Warpzone *warpzones, int &warpzone_count,
               BaseItem *items, int &item_count)
      : position(position), walls(walls), wall_positions(wall_positions),
        gates(gates), gate_positions(gate_positions), enemies(enemies),
        warpzones(warpzones), warpzone_count(warpzone_count), items(items),
        item_count(item_count), enemy_count(enemy_count) {}

  void visit(const EditorPlayer &player) override {
    // Handle player loading logic
  }

  void visit(const EditorVoid &voidCell) override {
    // Handle void cell loading logic
  }

  void visit(const EditorWall &wall) override {
    Wall w = (wall.type == UNBREAKABLE_WALL) ? create_ubreakable_wall(position)
                                             : create_breakable_wall(position);
    walls.push_back(w);
    wall_positions.push_back(w.position);
  }

  void visit(const EditorEnemy &enemy) override {
    Enemy e = create_enemy(position, enemy.type);
    enemies[enemy_count++] = e;
  }

  void visit(const EditorWarpzone &warpzone) override {
    warpzones[warpzone_count++] = {position, warpzone.destination};
  }

  void visit(const EditorItem &item) override {
    ItemTexture texture =
        (item.effect == HEALING_EFFECT) ? HEALING_CHIP_TEXTURE : KEY_TEXTURE;
    BaseItem i = create_base_item(item.effect, item.usage, texture, position);
    items[item_count++] = i;
  }

  void visit(const EditorGate &gate) override {
    Gate g;
    g.opened = false;
    g.position = position;
    g.type = BASE_GATE;
    gates.push_back(g);
    gate_positions.push_back(g.position);
  }

private:
  std::vector<Wall> &walls;
  std::vector<Vector2> &wall_positions;
  std::vector<Gate> &gates;
  std::vector<Vector2> &gate_positions;
  Enemy *enemies;
  Warpzone *warpzones;
  int &warpzone_count;
  BaseItem *items;
  int &item_count;
  int &enemy_count;
};
