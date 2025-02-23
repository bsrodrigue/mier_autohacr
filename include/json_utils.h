#pragma once

#include "cJSON.h"
#include "level_editor.h"
#include <raylib.h>
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

bool json_has_valid_number(cJSON *json, const char *field_name);

int json_get_int(cJSON *json, const char *field_name, int default_value = 0);

double json_get_double(cJSON *json, const char *field_name,
                       double default_value = 0.0);

bool json_validate_entity_type(cJSON *json, EntityType expected_type);

bool json_get_bool(cJSON *json, const char *key, bool default_value);

Vector2 parse_vector2(cJSON *json);
