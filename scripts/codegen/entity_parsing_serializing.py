
import json
import os

CONFIGS_FILE = "configs/entities/entities.json"

# Load entity metadata
with open(CONFIGS_FILE, "r") as f:
    entities = json.load(f)

# Header Template (Declarations)
header_template = """#pragma once

#include "cJSON.h"
#include "json_utils.h"

// Parsing Declarations
{parsing_declarations}

// Serialization Declarations
{serialization_declarations}
"""

# Source Template (Implementations)
source_template = """#include "json.h"

// Parsing Implementations
{parsing_implementations}

// Serialization Implementations
{serialization_implementations}
"""

# Function Templates
parse_impl_template = """
ParseResult parse_editor_{lower_name}(cJSON *json) {{
  if (!json) {{
    throw JsonParseException("Null JSON object passed to parse_editor_{lower_name}");
  }}
{required_checks}
  Vector2 position = parse_vector2(json);
{entity_construction}
{type_validation}
  return {{position, editor_{lower_name}, {entity_type_final}}};
}}
"""

serialize_impl_template = """
cJSON *serialize_editor_{lower_name}(const {entity_name} &editor_{lower_name}, int pos_y, int pos_x) {{
  cJSON *{lower_name}_json = cJSON_CreateObject();
  if (!{lower_name}_json) {{
    throw JsonParseException("Failed to create JSON object for {entity_name}");
  }}
{serialize_fields}
  cJSON_AddNumberToObject({lower_name}_json, "pos_x", pos_x);
  cJSON_AddNumberToObject({lower_name}_json, "pos_y", pos_y);
  cJSON_AddNumberToObject({lower_name}_json, "entity_type", {entity_type_final});
  return {lower_name}_json;
}}
"""

def generate_field_check(field):
    if field["required"]:
        if field["type"] == "Vector2":
            return f'  cJSON *{field["name"]}_json = cJSON_GetObjectItem(json, "{field["name"]}");\n  if (!{field["name"]}_json || !cJSON_IsObject({field["name"]}_json)) {{ throw JsonParseException("Missing or invalid \'{field["name"]}\' field in {lower_name}"); }}\n'
        else:
            return f'  if (!json_has_valid_number(json, "{field["name"]}")) {{ throw JsonParseException("Missing or invalid \'{field["name"]}\' field in {lower_name}"); }}\n'
    return ""

def generate_field_parse(field, is_required=False):
    if field["type"] == "Vector2":
        return f'  Vector2 {field["name"]} = parse_vector2({field["name"]}_json);\n'
    elif field["type"] == "bool":
        return f'  editor_{lower_name}.{field["name"]} = json_get_bool(json, "{field["name"]}", {field["default"]});\n'
    elif field["type"] == "float":
        default = field["default"] if field["default"] is not None else "0.0"
        return f'  editor_{lower_name}.{field["name"]} = (float)json_get_double(json, "{field["name"]}", {default});\n'
    elif is_required:
        if field["name"] == "entity_type":  # Special case for parsed entity_type
            return f'  {field["type"]} {field["name"]} = ({field["type"]})json_get_int(json, "{field["name"]}");\n'
        return f'  {field["type"]} {field["name"]} = ({field["type"]})json_get_int(json, "{field["name"]}");\n'
    else:
        default = field["default"] if field["default"] is not None else "0"
        return f'  editor_{lower_name}.{field["name"]} = ({field["type"]})json_get_double(json, "{field["name"]}", {default});\n'

def generate_serialize_field(field):
    if field["type"] == "Vector2":
        return f'  cJSON *{field["name"]}_json = serialize_vector2(editor_{lower_name}.{field["name"]});\n  if (!{field["name"]}_json) {{ cJSON_Delete({lower_name}_json); throw JsonParseException("Failed to serialize {field["name"]}"); }}\n  cJSON_AddItemToObject({lower_name}_json, "{field["name"]}", {field["name"]}_json);\n'
    elif field["type"] == "bool":
        return f'  cJSON_AddBoolToObject({lower_name}_json, "{field["name"]}", editor_{lower_name}.{field["name"]});\n'
    elif field["type"] == "float":
        return f'  cJSON_AddNumberToObject({lower_name}_json, "{field["name"]}", editor_{lower_name}.{field["name"]});\n'
    else:
        return f'  cJSON_AddNumberToObject({lower_name}_json, "{field["name"]}", static_cast<int>(editor_{lower_name}.{field["name"]}));\n'

# Generate Declarations
parsing_declarations = ""
serialization_declarations = ""
for entity in entities:
    entity_name = entity["name"]
    lower_name = entity_name.lower().replace("editor", "")
    parsing_declarations += f"ParseResult parse_editor_{lower_name}(cJSON *json);\n"
    serialization_declarations += f"cJSON *serialize_editor_{lower_name}(const {entity_name} &editor_{lower_name}, int pos_y, int pos_x);\n"

# Generate Implementations
parsing_implementations = ""
serialization_implementations = ""
for entity in entities:
    entity_name = entity["name"]
    lower_name = entity_name.lower().replace("editor", "")
    entity_type = entity["entity_type"]

    # Parsing Implementation
    required_checks = "".join(generate_field_check(f) for f in entity["fields"] if f["required"])
    required_fields = [f for f in entity["fields"] if f["required"]]
    optional_fields = [f for f in entity["fields"] if not f["required"]]
    required_parses = "".join(generate_field_parse(f, is_required=True) for f in required_fields)
    optional_parses = "".join(generate_field_parse(f) for f in optional_fields)

    if entity_name == "EditorWall":
        entity_construction = f"""  EditorWall editor_{lower_name}((WallType)json_get_int(json, "type"));
  WallType type = (WallType)json_get_int(json, "type");
  EntityType entity_type = (EntityType)json_get_int(json, "entity_type");
{optional_parses}"""
        type_validation = """  bool is_valid = (type == BREAKABLE_WALL && entity_type == BWALL_ENTITY) ||
                  (type == UNBREAKABLE_WALL && entity_type == UBWALL_ENTITY);
  if (!is_valid) { throw JsonParseException("Wall type and entity_type mismatch"); }
"""
        entity_type_final = "entity_type"
    elif required_fields:
        constructor_args = ", ".join(f["name"] for f in required_fields if f["name"] != "entity_type")
        entity_construction = f'  {required_parses}  {entity_name} editor_{lower_name}({constructor_args});\n{optional_parses}'
        type_validation = f'  if (!json_validate_entity_type(json, {entity_type})) {{ throw JsonParseException("Invalid entity type for {lower_name}"); }}\n'
        entity_type_final = entity_type
    else:
        entity_construction = f'  {entity_name} editor_{lower_name};\n{optional_parses}'
        type_validation = f'  if (!json_validate_entity_type(json, {entity_type})) {{ throw JsonParseException("Invalid entity type for {lower_name}"); }}\n'
        entity_type_final = entity_type

    parsing_implementations += parse_impl_template.format(
        lower_name=lower_name, entity_name=entity_name, entity_type=entity_type,
        required_checks=required_checks, entity_construction=entity_construction,
        type_validation=type_validation, entity_type_final=entity_type_final
    )

    # Serialization Implementation
    serialize_fields = "".join(generate_serialize_field(f) for f in entity["fields"] if f["name"] != "entity_type")
    if entity_name == "EditorWall":
        serialize_fields = f'  EntityType entity_type = (editor_{lower_name}.type == BREAKABLE_WALL) ? BWALL_ENTITY : UBWALL_ENTITY;\n' + serialize_fields
        entity_type_final = "entity_type"
    else:
        entity_type_final = entity_type

    serialization_implementations += serialize_impl_template.format(
        entity_name=entity_name, lower_name=lower_name, entity_type=entity_type,
        serialize_fields=serialize_fields, entity_type_final=entity_type_final
    )

# Write Header File
with open("include/json.h", "w") as f:
    f.write(header_template.format(
        parsing_declarations=parsing_declarations,
        serialization_declarations=serialization_declarations
    ))

# Write Source File
with open("src/json.cc", "w") as f:
    f.write(source_template.format(
        parsing_implementations=parsing_implementations,
        serialization_implementations=serialization_implementations
    ))

print("Generated code written to json.h and json.cc")
