#pragma once

#include "cJSON.h"
#include "level_editor.h"
#include <raylib.h>

struct ParseResult {
  Vector2 position;
  std::variant<EditorVoid, EditorWall, EditorEnemy, EditorWarpzone,
               EditorPlayer, EditorItem, EditorGate>
      entity;
  EntityType type;
};

Vector2 parse_vector2(cJSON *json);

ParseResult parse_editor_void(cJSON *json);

ParseResult parse_editor_warpzone(cJSON *json);

ParseResult parse_editor_item(cJSON *json);

ParseResult parse_editor_wall(cJSON *json);

ParseResult parse_editor_enemy(cJSON *json);

ParseResult parse_editor_gate(cJSON *json);

ParseResult parse_editor_player(cJSON *json);

cJSON *serialize_vector2(const Vector2 &vec);

cJSON *serialize_editor_void(const EditorVoid &entity, int pos_y, int pos_x);

cJSON *serialize_editor_wall(const EditorWall &wall, int pos_y, int pos_x);

cJSON *serialize_editor_enemy(const EditorEnemy &enemy, int pos_y, int pos_x);

cJSON *serialize_editor_warpzone(const EditorWarpzone &warpzone, int pos_y,
                                 int pos_x);

cJSON *serialize_editor_item(const EditorItem &item, int pos_y, int pos_x);

cJSON *serialize_editor_gate(const EditorGate &gate, int pos_y, int pos_x);

cJSON *serialize_editor_player(const EditorPlayer &player, int pos_y,
                               int pos_x);
