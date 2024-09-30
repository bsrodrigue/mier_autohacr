#pragma once

#include "json.h"
#include "level_editor.h"

Vector2 parse_vector2(cJSON *json) {
  Vector2 vec = {0, 0}; // Default value
  //
  if (cJSON_IsObject(json)) {
    cJSON *x = cJSON_GetObjectItem(json, "pos_x");
    cJSON *y = cJSON_GetObjectItem(json, "pos_y");
    if (cJSON_IsNumber(x) && cJSON_IsNumber(y)) {
      vec.x = (float)x->valuedouble;
      vec.y = (float)y->valuedouble;
    }
  }
  return vec;
}

ParseResult parse_editor_void(cJSON *json) {
  Vector2 position = parse_vector2(json);
  return {position, EditorVoid()};
}

ParseResult parse_editor_warpzone(cJSON *json) {
  Vector2 destination = parse_vector2(cJSON_GetObjectItem(json, "destination"));
  Vector2 position = parse_vector2(json);
  return {position, EditorWarpzone(destination)};
}

ParseResult parse_editor_item(cJSON *json) {
  ItemEffect effect = (ItemEffect)cJSON_GetObjectItem(json, "effect")
                          ->valueint; // Assuming it's an integer
  ItemUsage usage = (ItemUsage)cJSON_GetObjectItem(json, "usage")
                        ->valueint; // Assuming it's an integer
  Vector2 position = parse_vector2(json);
  return {position, EditorItem(effect, usage)};
}

ParseResult parse_editor_wall(cJSON *json) {
  WallType type = (WallType)cJSON_GetObjectItem(json, "type")
                      ->valueint; // Assuming it's an integer
  Vector2 position = parse_vector2(json);
  return {position, EditorWall(type)};
}

ParseResult parse_editor_enemy(cJSON *json) {
  EnemyType type = (EnemyType)cJSON_GetObjectItem(json, "type")
                       ->valueint; // Assuming it's an integer
  Vector2 position = parse_vector2(json);
  return {position, EditorEnemy(type)};
}

ParseResult parse_editor_gate(cJSON *json) {
  Vector2 position = parse_vector2(json);
  return {position, EditorGate()};
}

ParseResult parse_editor_player(cJSON *json) {
  Vector2 position = parse_vector2(json);
  return {position, EditorPlayer()};
}

cJSON *serialize_vector2(const Vector2 &vec) {
  cJSON *json_vector = cJSON_CreateObject();
  cJSON_AddNumberToObject(json_vector, "pos_x", vec.x);
  cJSON_AddNumberToObject(json_vector, "pos_y", vec.y);
  return json_vector;
}

// Serialize function for EditorVoid
cJSON *serialize_editor_void(const EditorVoid &entity, int pos_y, int pos_x) {
  cJSON *json_void = cJSON_CreateObject();
  cJSON_AddNumberToObject(json_void, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_void, "pos_y", pos_y);
  return json_void;
}

// Serialize function for EditorWall
cJSON *serialize_editor_wall(const EditorWall &wall, int pos_y, int pos_x) {
  cJSON *json_wall = cJSON_CreateObject();
  cJSON_AddNumberToObject(
      json_wall, "type",
      static_cast<int>(wall.type)); // Assuming WallType is an enum
  cJSON_AddNumberToObject(json_wall, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_wall, "pos_y", pos_y);
  return json_wall;
}

// Serialize function for EditorEnemy
cJSON *serialize_editor_enemy(const EditorEnemy &enemy, int pos_y, int pos_x) {
  cJSON *json_enemy = cJSON_CreateObject();
  cJSON_AddNumberToObject(
      json_enemy, "type",
      static_cast<int>(enemy.type)); // Assuming EnemyType is an enum
  cJSON_AddNumberToObject(json_enemy, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_enemy, "pos_y", pos_y);
  return json_enemy;
}

// Serialize function for EditorWarpzone
cJSON *serialize_editor_warpzone(const EditorWarpzone &warpzone, int pos_y,
                                 int pos_x) {
  cJSON *json_warpzone = cJSON_CreateObject();
  cJSON_AddItemToObject(json_warpzone, "destination",
                        serialize_vector2(warpzone.destination));
  cJSON_AddNumberToObject(json_warpzone, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_warpzone, "pos_y", pos_y);
  return json_warpzone;
}

// Serialize function for EditorItem
cJSON *serialize_editor_item(const EditorItem &item, int pos_y, int pos_x) {
  cJSON *json_item = cJSON_CreateObject();
  cJSON_AddNumberToObject(
      json_item, "effect",
      static_cast<int>(item.effect)); // Assuming ItemEffect is an enum
  cJSON_AddNumberToObject(
      json_item, "usage",
      static_cast<int>(item.usage)); // Assuming ItemUsage is an enum
  cJSON_AddNumberToObject(json_item, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_item, "pos_y", pos_y);
  return json_item;
}

// Serialize function for EditorGate
cJSON *serialize_editor_gate(const EditorGate &gate, int pos_y, int pos_x) {
  cJSON *json_gate = cJSON_CreateObject();
  cJSON_AddNumberToObject(json_gate, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_gate, "pos_y", pos_y);
  return json_gate;
}

// Serialize function for EditorPlayer
cJSON *serialize_editor_player(const EditorPlayer &player, int pos_y,
                               int pos_x) {
  cJSON *json_player = cJSON_CreateObject();
  cJSON_AddNumberToObject(json_player, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_player, "pos_y", pos_y);
  return json_player;
}
