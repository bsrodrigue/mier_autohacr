import json
import os

# Load entity metadata
with open("entities.json", "r") as f:
    entities = json.load(f)

# Templates
parse_template = """
ParseResult parse_editor_{lower_name}(cJSON *json) {{
  if (!json) {{
    throw JsonParseException("Null JSON object passed to parse_editor_{lower_name}");
  }}
{required_checks}
  Vector2 position = parse_vector2(json);
{entity_construction}
{type_validation}
  return {{position, {entity_var}, {entity_type}}};
}}
"""

serialize_template = """
cJSON *serialize_editor_{lower_name}(const {entity_name} &{lower_name}, int pos_y, int pos_x) {{
  cJSON *{lower_name}_json = cJSON_CreateObject();
  if (!{lower_name}_json) {{
    throw JsonParseException("Failed to create JSON object for {entity_name}");
  }}
{serialize_fields}
  cJSON_AddNumberToObject({lower_name}_json, "pos_x", pos_x);
  cJSON_AddNumberToObject({lower_name}_json, "pos_y", pos_y);
  cJSON_AddNumberToObject({lower_name}_json, "entity_type", {entity_type});
  return {lower_name}_json;
}}
"""

def generate_field_check(field):
    if field["required"]:
        return f'  if (!json_has_valid_number(json, "{field["name"]}") {{ throw JsonParseException("Missing or invalid \'{field["name"]}\' field in {lower_name}"); }}\n'
    return ""

def generate_field_parse(field):
    if field["type"] == "Vector2":
        return f'  cJSON *{field["name"]}_json = cJSON_GetObjectItem(json, "{field["name"]}");\n  if (!{field["name"]}_json || !cJSON_IsObject({field["name"]}_json)) {{ throw JsonParseException("Missing or invalid \'{field["name"]}\' field in {lower_name}"); }}\n  Vector2 {field["name"]} = parse_vector2({field["name"]}_json);\n'
    elif field["type"] == "bool":
        return f'  {lower_name}.{field["name"]} = json_get_bool(json, "{field["name"]}", {field["default"]});\n'
    elif field["required"]:
        return f'  {field["type"]} {field["name"]} = ({field["type"]})json_get_int(json, "{field["name"]}");\n'
    else:
        default = field["default"] if field["default"] is not None else "0"
        return f'  {lower_name}.{field["name"]} = ({field["type"]})json_get_double(json, "{field["name"]}", {default});\n'

def generate_serialize_field(field):
    if field["type"] == "Vector2":
        return f'  cJSON *{field["name"]}_json = serialize_vector2({lower_name}.{field["name"]});\n  if (!{field["name"]}_json) {{ cJSON_Delete({lower_name}_json); throw JsonParseException("Failed to serialize {field["name"]}"); }}\n  cJSON_AddItemToObject({lower_name}_json, "{field["name"]}", {field["name"]}_json);\n'
    elif field["type"] == "bool":
        return f'  cJSON_AddBoolToObject({lower_name}_json, "{field["name"]}", {lower_name}.{field["name"]});\n'
    else:
        return f'  cJSON_AddNumberToObject({lower_name}_json, "{field["name"]}", static_cast<int>({lower_name}.{field["name"]}));\n'

# Generate code
output = "#pragma once\n\n#include \"json.h\"\n#include \"cJSON.h\"\n#include \"json_utils.h\"\n#include \"level_editor.h\"\n\n// Parsing\n"
for entity in entities:
    entity_name = entity["name"]
    lower_name = entity_name.lower().replace("editor", "")
    entity_type = entity["entity_type"]

    # Parsing
    required_checks = "".join(generate_field_check(f) for f in entity["fields"] if f["required"])
    field_parses = "".join(generate_field_parse(f) for f in entity["fields"])
    entity_var = lower_name if entity["fields"] else f'{entity_name}()'
    entity_construction = f'  {entity_name} {lower_name}{"()" if not entity["fields"] else ""};\n{field_parses}' if entity["fields"] else ""
    type_validation = f'  if (!json_validate_entity_type(json, {entity_type})) {{ throw JsonParseException("Invalid entity type for {lower_name}"); }}\n' if " or " not in entity_type else ""
    output += parse_template.format(
        lower_name=lower_name, entity_name=entity_name, entity_type=entity_type,
        required_checks=required_checks, entity_construction=entity_construction,
        type_validation=type_validation, entity_var=entity_var
    )

output += "\n// Serializing\n"
for entity in entities:
    entity_name = entity["name"]
    lower_name = entity_name.lower().replace("editor", "")
    entity_type = entity["entity_type"]
    serialize_fields = "".join(generate_serialize_field(f) for f in entity["fields"])
    output += serialize_template.format(
        entity_name=entity_name, lower_name=lower_name, entity_type=entity_type,
        serialize_fields=serialize_fields
    )

# Write to file
with open("generated_json_handlers.h", "w") as f:
    f.write(output)

print("Generated code written to generated_json_handlers.h")
