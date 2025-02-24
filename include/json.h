#pragma once

#include "cJSON.h"
#include "json_utils.h"

// Parsing Declarations
ParseResult parse_editor_void(cJSON *json);
ParseResult parse_editor_enemy(cJSON *json);
ParseResult parse_editor_wall(cJSON *json);
ParseResult parse_editor_warpzone(cJSON *json);
ParseResult parse_editor_item(cJSON *json);
ParseResult parse_editor_gate(cJSON *json);
ParseResult parse_editor_player(cJSON *json);


// Serialization Declarations
cJSON *serialize_editor_void(const EditorVoid &editor_void, int pos_y, int pos_x);
cJSON *serialize_editor_enemy(const EditorEnemy &editor_enemy, int pos_y, int pos_x);
cJSON *serialize_editor_wall(const EditorWall &editor_wall, int pos_y, int pos_x);
cJSON *serialize_editor_warpzone(const EditorWarpzone &editor_warpzone, int pos_y, int pos_x);
cJSON *serialize_editor_item(const EditorItem &editor_item, int pos_y, int pos_x);
cJSON *serialize_editor_gate(const EditorGate &editor_gate, int pos_y, int pos_x);
cJSON *serialize_editor_player(const EditorPlayer &editor_player, int pos_y, int pos_x);

