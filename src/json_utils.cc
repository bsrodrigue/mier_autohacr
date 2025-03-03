#include "json_utils.h"

//============================================== Utility functions
//==============================================//


bool json_has_valid_number(cJSON *json, const char *field_name) {
  if (!json)
    return false;
  cJSON *item = cJSON_GetObjectItem(json, field_name);
  return item && cJSON_IsNumber(item);
}

int json_get_int(cJSON *json, const char *field_name, int default_value) {
  if (!json_has_valid_number(json, field_name))
    return default_value;
  return cJSON_GetObjectItem(json, field_name)->valueint;
}

double json_get_double(cJSON *json, const char *field_name,
                       double default_value) {
  if (!json_has_valid_number(json, field_name))
    return default_value;
  return cJSON_GetObjectItem(json, field_name)->valuedouble;
}

bool json_validate_entity_type(cJSON *json, EntityType expected_type) {
  int entity_type = json_get_int(json, "entity_type", -1);
  return entity_type == expected_type;
}

bool json_get_bool(cJSON *json, const char *key, bool default_value) {
  cJSON *item = cJSON_GetObjectItem(json, key);
  return item ? cJSON_IsTrue(item) : default_value;
}

Vector2 parse_vector2(cJSON *json) {
  Vector2 vec = {0, 0}; // Default value
  if (!json || !cJSON_IsObject(json)) {
    return vec;
  }

  vec.x = (float)json_get_double(json, "pos_x");
  vec.y = (float)json_get_double(json, "pos_y");
  return vec;
}

cJSON *serialize_vector2(const Vector2 &vec) {
  cJSON *json_vector = cJSON_CreateObject();
  if (!json_vector) {
    throw JsonParseException("Failed to create JSON object for Vector2");
  }

  cJSON_AddNumberToObject(json_vector, "pos_x", vec.x);
  cJSON_AddNumberToObject(json_vector, "pos_y", vec.y);
  return json_vector;
}
