#pragma once

#include "json.h"
#include "cJSON.h"
#include "level_editor.h"
#include <stdexcept>
#include <string>

//============================================== Error handling
//==============================================//

class JsonParseException : public std::runtime_error {
public:
  JsonParseException(const std::string &what) : std::runtime_error(what) {}
};

//============================================== Utility functions
//==============================================//

bool json_has_valid_number(cJSON *json, const char *field_name) {
  if (!json)
    return false;
  cJSON *item = cJSON_GetObjectItem(json, field_name);
  return item && cJSON_IsNumber(item);
}

int json_get_int(cJSON *json, const char *field_name, int default_value = 0) {
  if (!json_has_valid_number(json, field_name))
    return default_value;
  return cJSON_GetObjectItem(json, field_name)->valueint;
}

double json_get_double(cJSON *json, const char *field_name,
                       double default_value = 0.0) {
  if (!json_has_valid_number(json, field_name))
    return default_value;
  return cJSON_GetObjectItem(json, field_name)->valuedouble;
}

bool json_validate_entity_type(cJSON *json, EntityType expected_type) {
  int entity_type = json_get_int(json, "entity_type", -1);
  return entity_type == expected_type;
}

//============================================== Parsing
//==============================================//

Vector2 parse_vector2(cJSON *json) {
  Vector2 vec = {0, 0}; // Default value
  if (!json || !cJSON_IsObject(json)) {
    return vec;
  }

  vec.x = (float)json_get_double(json, "pos_x");
  vec.y = (float)json_get_double(json, "pos_y");
  return vec;
}

ParseResult parse_editor_void(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_void");
  }

  Vector2 position = parse_vector2(json);
  return {position, EditorVoid(), EMPTY_ENTITY};
}

ParseResult parse_editor_warpzone(cJSON *json) {
  if (!json) {
    throw JsonParseException(
        "Null JSON object passed to parse_editor_warpzone");
  }

  cJSON *dest_json = cJSON_GetObjectItem(json, "destination");
  if (!dest_json || !cJSON_IsObject(dest_json)) {
    throw JsonParseException(
        "Missing or invalid 'destination' field in warpzone");
  }

  Vector2 destination = parse_vector2(dest_json);
  Vector2 position = parse_vector2(json);

  if (!json_validate_entity_type(json, WARPZONE_ENTITY)) {
    throw JsonParseException("Invalid entity type for warpzone");
  }

  return {position, EditorWarpzone(destination), WARPZONE_ENTITY};
}

ParseResult parse_editor_item(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_item");
  }

  if (!json_has_valid_number(json, "effect") ||
      !json_has_valid_number(json, "usage")) {
    throw JsonParseException(
        "Missing or invalid 'effect' or 'usage' field in item");
  }

  ItemEffect effect = (ItemEffect)json_get_int(json, "effect");
  ItemUsage usage = (ItemUsage)json_get_int(json, "usage");
  Vector2 position = parse_vector2(json);

  if (!json_validate_entity_type(json, ITEM_ENTITY)) {
    throw JsonParseException("Invalid entity type for item");
  }

  return {position, EditorItem(effect, usage), ITEM_ENTITY};
}

ParseResult parse_editor_wall(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_wall");
  }

  if (!json_has_valid_number(json, "type")) {
    throw JsonParseException("Missing or invalid 'type' field in wall");
  }

  WallType type = (WallType)json_get_int(json, "type");
  EntityType entity_type = (EntityType)json_get_int(json, "entity_type");

  // Validate that entity_type matches wall type
  bool is_valid = (type == BREAKABLE_WALL && entity_type == BWALL_ENTITY) ||
                  (type == UNBREAKABLE_WALL && entity_type == UBWALL_ENTITY);

  if (!is_valid) {
    throw JsonParseException("Wall type and entity_type mismatch");
  }

  Vector2 position = parse_vector2(json);
  return {position, EditorWall(type), entity_type};
}

ParseResult parse_editor_enemy(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_enemy");
  }

  if (!json_has_valid_number(json, "type")) {
    throw JsonParseException("Missing or invalid 'type' field in enemy");
  }

  EnemyType type = (EnemyType)json_get_int(json, "type");
  Vector2 position = parse_vector2(json);

  if (!json_validate_entity_type(json, BASE_ENEMY_ENTITY)) {
    throw JsonParseException("Invalid entity type for enemy");
  }

  float enemy_health = (float)json_get_double(json, "enemy_health", 1.0);

  return {position, EditorEnemy(type, enemy_health), BASE_ENEMY_ENTITY};
}

ParseResult parse_editor_gate(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_gate");
  }

  Vector2 position = parse_vector2(json);

  if (!json_validate_entity_type(json, GATE_ENTITY)) {
    throw JsonParseException("Invalid entity type for gate");
  }

  return {position, EditorGate(), GATE_ENTITY};
}

ParseResult parse_editor_player(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_player");
  }

  Vector2 position = parse_vector2(json);

  if (!json_validate_entity_type(json, PLAYER_ENTITY)) {
    throw JsonParseException("Invalid entity type for player");
  }

  return {position, EditorPlayer(), PLAYER_ENTITY};
}

//============================================== Serializing
//==============================================//

cJSON *serialize_vector2(const Vector2 &vec) {
  cJSON *json_vector = cJSON_CreateObject();
  if (!json_vector) {
    throw JsonParseException("Failed to create JSON object for Vector2");
  }

  cJSON_AddNumberToObject(json_vector, "pos_x", vec.x);
  cJSON_AddNumberToObject(json_vector, "pos_y", vec.y);
  return json_vector;
}

cJSON *serialize_editor_void(const EditorVoid &entity, int pos_y, int pos_x) {
  cJSON *json_void = cJSON_CreateObject();
  if (!json_void) {
    throw JsonParseException("Failed to create JSON object for EditorVoid");
  }

  cJSON_AddNumberToObject(json_void, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_void, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_void, "entity_type", EMPTY_ENTITY);
  return json_void;
}

cJSON *serialize_editor_wall(const EditorWall &wall, int pos_y, int pos_x) {
  cJSON *json_wall = cJSON_CreateObject();
  if (!json_wall) {
    throw JsonParseException("Failed to create JSON object for EditorWall");
  }

  EntityType entity_type =
      (wall.type == BREAKABLE_WALL) ? BWALL_ENTITY : UBWALL_ENTITY;

  cJSON_AddNumberToObject(json_wall, "type", static_cast<int>(wall.type));
  cJSON_AddNumberToObject(json_wall, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_wall, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_wall, "entity_type", entity_type);
  return json_wall;
}

cJSON *serialize_editor_enemy(const EditorEnemy &enemy, int pos_y, int pos_x) {
  cJSON *json_enemy = cJSON_CreateObject();
  if (!json_enemy) {
    throw JsonParseException("Failed to create JSON object for EditorEnemy");
  }

  cJSON_AddNumberToObject(json_enemy, "type", static_cast<int>(enemy.type));
  cJSON_AddNumberToObject(json_enemy, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_enemy, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_enemy, "entity_type", BASE_ENEMY_ENTITY);
  cJSON_AddNumberToObject(json_enemy, "enemy_health", enemy.enemy_health);
  return json_enemy;
}

cJSON *serialize_editor_warpzone(const EditorWarpzone &warpzone, int pos_y,
                                 int pos_x) {
  cJSON *json_warpzone = cJSON_CreateObject();
  if (!json_warpzone) {
    throw JsonParseException("Failed to create JSON object for EditorWarpzone");
  }

  cJSON *destination_json = serialize_vector2(warpzone.destination);
  if (!destination_json) {
    cJSON_Delete(json_warpzone);
    throw JsonParseException("Failed to serialize warpzone destination");
  }

  cJSON_AddItemToObject(json_warpzone, "destination", destination_json);
  cJSON_AddNumberToObject(json_warpzone, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_warpzone, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_warpzone, "entity_type", WARPZONE_ENTITY);
  return json_warpzone;
}

cJSON *serialize_editor_item(const EditorItem &item, int pos_y, int pos_x) {
  cJSON *json_item = cJSON_CreateObject();
  if (!json_item) {
    throw JsonParseException("Failed to create JSON object for EditorItem");
  }

  cJSON_AddNumberToObject(json_item, "effect", static_cast<int>(item.effect));
  cJSON_AddNumberToObject(json_item, "usage", static_cast<int>(item.usage));
  cJSON_AddNumberToObject(json_item, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_item, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_item, "entity_type", ITEM_ENTITY);
  return json_item;
}

cJSON *serialize_editor_gate(const EditorGate &gate, int pos_y, int pos_x) {
  cJSON *json_gate = cJSON_CreateObject();
  if (!json_gate) {
    throw JsonParseException("Failed to create JSON object for EditorGate");
  }

  cJSON_AddNumberToObject(json_gate, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_gate, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_gate, "entity_type", GATE_ENTITY);
  return json_gate;
}

cJSON *serialize_editor_player(const EditorPlayer &player, int pos_y,
                               int pos_x) {
  cJSON *json_player = cJSON_CreateObject();
  if (!json_player) {
    throw JsonParseException("Failed to create JSON object for EditorPlayer");
  }

  cJSON_AddNumberToObject(json_player, "pos_x", pos_x);
  cJSON_AddNumberToObject(json_player, "pos_y", pos_y);
  cJSON_AddNumberToObject(json_player, "entity_type", PLAYER_ENTITY);
  return json_player;
}
