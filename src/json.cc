#include "json.h"

// Parsing Implementations

ParseResult parse_editor_void(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_void");
  }

  Vector2 position = parse_vector2(json);
  EditorVoid editor_void;

  if (!json_validate_entity_type(json, EMPTY_ENTITY)) { throw JsonParseException("Invalid entity type for void"); }

  return {position, editor_void, EMPTY_ENTITY};
}

ParseResult parse_editor_enemy(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_enemy");
  }

  Vector2 position = parse_vector2(json);
  EditorEnemy editor_enemy;
  editor_enemy.type = (EnemyType)json_get_double(json, "type", BASE_ENEMY);
  editor_enemy.enemy_health = (float)json_get_double(json, "enemy_health", 0.0);
  editor_enemy.shooting_interval = (float)json_get_double(json, "shooting_interval", 0.0);
  editor_enemy.shooting_duration = (float)json_get_double(json, "shooting_duration", 5.0);
  editor_enemy.shooting_cooldown = (float)json_get_double(json, "shooting_cooldown", 2.0);
  editor_enemy.tracks_player = json_get_bool(json, "tracks_player", false);
  editor_enemy.follows_player = json_get_bool(json, "follows_player", false);

  if (!json_validate_entity_type(json, BASE_ENEMY_ENTITY)) { throw JsonParseException("Invalid entity type for enemy"); }

  return {position, editor_enemy, BASE_ENEMY_ENTITY};
}

ParseResult parse_editor_wall(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_wall");
  }
  if (!json_has_valid_number(json, "type")) { throw JsonParseException("Missing or invalid 'type' field in wall"); }
  if (!json_has_valid_number(json, "entity_type")) { throw JsonParseException("Missing or invalid 'entity_type' field in wall"); }

  Vector2 position = parse_vector2(json);
  EditorWall editor_wall((WallType)json_get_int(json, "type"));
  WallType type = (WallType)json_get_int(json, "type");
  EntityType entity_type = (EntityType)json_get_int(json, "entity_type");

  bool is_valid = (type == BREAKABLE_WALL && entity_type == BWALL_ENTITY) ||
                  (type == UNBREAKABLE_WALL && entity_type == UBWALL_ENTITY);
  if (!is_valid) { throw JsonParseException("Wall type and entity_type mismatch"); }

  return {position, editor_wall, entity_type};
}

ParseResult parse_editor_warpzone(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_warpzone");
  }
  cJSON *destination_json = cJSON_GetObjectItem(json, "destination");
  if (!destination_json || !cJSON_IsObject(destination_json)) { throw JsonParseException("Missing or invalid 'destination' field in warpzone"); }

  Vector2 position = parse_vector2(json);
    Vector2 destination = parse_vector2(destination_json);
  EditorWarpzone editor_warpzone(destination);

  if (!json_validate_entity_type(json, WARPZONE_ENTITY)) { throw JsonParseException("Invalid entity type for warpzone"); }

  return {position, editor_warpzone, WARPZONE_ENTITY};
}

ParseResult parse_editor_item(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_item");
  }
  if (!json_has_valid_number(json, "effect")) { throw JsonParseException("Missing or invalid 'effect' field in item"); }
  if (!json_has_valid_number(json, "usage")) { throw JsonParseException("Missing or invalid 'usage' field in item"); }

  Vector2 position = parse_vector2(json);
    ItemEffect effect = (ItemEffect)json_get_int(json, "effect");
  ItemUsage usage = (ItemUsage)json_get_int(json, "usage");
  EditorItem editor_item(effect, usage);

  if (!json_validate_entity_type(json, ITEM_ENTITY)) { throw JsonParseException("Invalid entity type for item"); }

  return {position, editor_item, ITEM_ENTITY};
}

ParseResult parse_editor_gate(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_gate");
  }

  Vector2 position = parse_vector2(json);
  EditorGate editor_gate;

  if (!json_validate_entity_type(json, GATE_ENTITY)) { throw JsonParseException("Invalid entity type for gate"); }

  return {position, editor_gate, GATE_ENTITY};
}

ParseResult parse_editor_player(cJSON *json) {
  if (!json) {
    throw JsonParseException("Null JSON object passed to parse_editor_player");
  }

  Vector2 position = parse_vector2(json);
  EditorPlayer editor_player;

  if (!json_validate_entity_type(json, PLAYER_ENTITY)) { throw JsonParseException("Invalid entity type for player"); }

  return {position, editor_player, PLAYER_ENTITY};
}


// Serialization Implementations

cJSON *serialize_editor_void(const EditorVoid &editor_void, int pos_y, int pos_x) {
  cJSON *void_json = cJSON_CreateObject();
  if (!void_json) {
    throw JsonParseException("Failed to create JSON object for EditorVoid");
  }

  cJSON_AddNumberToObject(void_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(void_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(void_json, "entity_type", EMPTY_ENTITY);
  return void_json;
}

cJSON *serialize_editor_enemy(const EditorEnemy &editor_enemy, int pos_y, int pos_x) {
  cJSON *enemy_json = cJSON_CreateObject();
  if (!enemy_json) {
    throw JsonParseException("Failed to create JSON object for EditorEnemy");
  }
  cJSON_AddNumberToObject(enemy_json, "type", static_cast<int>(editor_enemy.type));
  cJSON_AddNumberToObject(enemy_json, "enemy_health", editor_enemy.enemy_health);
  cJSON_AddNumberToObject(enemy_json, "shooting_interval", editor_enemy.shooting_interval);
  cJSON_AddNumberToObject(enemy_json, "shooting_duration", editor_enemy.shooting_duration);
  cJSON_AddNumberToObject(enemy_json, "shooting_cooldown", editor_enemy.shooting_cooldown);
  cJSON_AddBoolToObject(enemy_json, "tracks_player", editor_enemy.tracks_player);
  cJSON_AddBoolToObject(enemy_json, "follows_player", editor_enemy.follows_player);

  cJSON_AddNumberToObject(enemy_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(enemy_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(enemy_json, "entity_type", BASE_ENEMY_ENTITY);
  return enemy_json;
}

cJSON *serialize_editor_wall(const EditorWall &editor_wall, int pos_y, int pos_x) {
  cJSON *wall_json = cJSON_CreateObject();
  if (!wall_json) {
    throw JsonParseException("Failed to create JSON object for EditorWall");
  }
  EntityType entity_type = (editor_wall.type == BREAKABLE_WALL) ? BWALL_ENTITY : UBWALL_ENTITY;
  cJSON_AddNumberToObject(wall_json, "type", static_cast<int>(editor_wall.type));

  cJSON_AddNumberToObject(wall_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(wall_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(wall_json, "entity_type", entity_type);
  return wall_json;
}

cJSON *serialize_editor_warpzone(const EditorWarpzone &editor_warpzone, int pos_y, int pos_x) {
  cJSON *warpzone_json = cJSON_CreateObject();
  if (!warpzone_json) {
    throw JsonParseException("Failed to create JSON object for EditorWarpzone");
  }
  cJSON *destination_json = serialize_vector2(editor_warpzone.destination);
  if (!destination_json) { cJSON_Delete(warpzone_json); throw JsonParseException("Failed to serialize destination"); }
  cJSON_AddItemToObject(warpzone_json, "destination", destination_json);

  cJSON_AddNumberToObject(warpzone_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(warpzone_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(warpzone_json, "entity_type", WARPZONE_ENTITY);
  return warpzone_json;
}

cJSON *serialize_editor_item(const EditorItem &editor_item, int pos_y, int pos_x) {
  cJSON *item_json = cJSON_CreateObject();
  if (!item_json) {
    throw JsonParseException("Failed to create JSON object for EditorItem");
  }
  cJSON_AddNumberToObject(item_json, "effect", static_cast<int>(editor_item.effect));
  cJSON_AddNumberToObject(item_json, "usage", static_cast<int>(editor_item.usage));

  cJSON_AddNumberToObject(item_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(item_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(item_json, "entity_type", ITEM_ENTITY);
  return item_json;
}

cJSON *serialize_editor_gate(const EditorGate &editor_gate, int pos_y, int pos_x) {
  cJSON *gate_json = cJSON_CreateObject();
  if (!gate_json) {
    throw JsonParseException("Failed to create JSON object for EditorGate");
  }

  cJSON_AddNumberToObject(gate_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(gate_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(gate_json, "entity_type", GATE_ENTITY);
  return gate_json;
}

cJSON *serialize_editor_player(const EditorPlayer &editor_player, int pos_y, int pos_x) {
  cJSON *player_json = cJSON_CreateObject();
  if (!player_json) {
    throw JsonParseException("Failed to create JSON object for EditorPlayer");
  }

  cJSON_AddNumberToObject(player_json, "pos_x", pos_x);
  cJSON_AddNumberToObject(player_json, "pos_y", pos_y);
  cJSON_AddNumberToObject(player_json, "entity_type", PLAYER_ENTITY);
  return player_json;
}

