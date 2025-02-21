#include "entities.h"
#include <string>
#include <vector>

const char *get_entity_type_name(EntityType type) {
  switch (type) {
  case EMPTY_ENTITY:
    return "Empty";
  case UBWALL_ENTITY:
    return "Unbreakable Wall";
  case BWALL_ENTITY:
    return "Breakable Wall";
  case PLAYER_ENTITY:
    return "Player";
  case BASE_ENEMY_ENTITY:
    return "Base Enemy";
  case WARPZONE_ENTITY:
    return "Warp Zone";
  case ITEM_ENTITY:
    return "Item";
  case GATE_ENTITY:
    return "GATE";
  default:
    return "";
  }
}

// Returns a string with all the entity names separated by a delimiter
std::string get_enum_string(const char delimiter) {
  std::vector<std::string> names;

  for (int i = EMPTY_ENTITY; i <= GATE_ENTITY; i++) {
    names.push_back(get_entity_type_name((EntityType)i));
  }

  std::string result = "";
  for (int i = 0; i < names.size(); i++) {
    result += names[i];
    if (i < names.size() - 1)
      result += delimiter;
  }

  return result;
}
