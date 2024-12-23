﻿﻿// MapConstants.h

#ifndef __MAP_CONSTANTS_H__

#define __MAP_CONSTANTS_H__

#include "cocos2d.h"

namespace MapConstants {

// GameTypes.h 或 MapTypes.h

enum class MapType {

  NONE,

  FARM, // 农场

  PLAZA, // 广场

  SHOP, // 商店

  HOMESTEAD, // 家园

  CAVE, // 洞穴

  INSIDE // 室内

};

} // namespace MapConstants

struct LayerProperties {

  bool base = false; // 可以走

  bool boundary = false; // 不可走且无物理属性

  bool physical = false; // 有物理属性

  bool penetrable = false; // 可穿过且会隐身

  bool arable = false;

  bool fishing = false;

  bool get_ore = false;

  bool canBeAttackted = false;

  bool canAttack = false;

  bool can_interactive = false;
};

#endif