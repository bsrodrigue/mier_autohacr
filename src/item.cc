#include "item.h"
#include <raylib.h>
#include <vector>

const char *get_item_effect_name(ItemEffect effect) {
  switch (effect) {
  case NO_EFFECT:
    return "No effect";
  case HEALING_EFFECT:
    return "Healing effect";
  case PROJECTILE_BOOST_EFFECT:
    return "Projectile Boost effect";
  case KEY_EFFECT:
    return "Key effect";
  case SPECIAL_EFFECT:
    return "Special effect";
  }
}

BaseItem create_base_item(ItemEffect effect, ItemUsage usage,
                          ItemTexture texture, Vector2 position) {
  BaseItem item;

  item.effect = effect;
  item.usage = usage;
  item.texture = texture;

  item.position = position;
  item.picked = false;
  item.used = false;

  return item;
}

std::string get_item_effect_enum_string(const char delimiter) {
  std::vector<std::string> names;

  for (int i = NO_EFFECT; i <= SPECIAL_EFFECT; i++) {
    names.push_back(get_item_effect_name((ItemEffect)i));
  }

  std::string result = "";
  for (int i = 0; i < names.size(); i++) {
    result += names[i];
    if (i < names.size() - 1)
      result += delimiter;
  }

  return result;
}
