// FarmMapManager.h
#pragma once
#include "FarmConfigManager.h"
#include "cocos2d.h"

USING_NS_CC;

class FarmMapManager {
public:
  static FarmMapManager *getInstance();

  bool initWithFarmType(const std::string &farmType);
  TMXTiledMap *getMap() const { return map; }

  // 地图查询接口
  bool isWalkable(const Vec2 &worldPos) const;
  bool isPenetrable(const Vec2 &worldPos) const;
  bool isBoundary(const Vec2 &worldPos) const;
  bool isPhysical(const Vec2 &worldPos) const;
  bool isBase(const Vec2 &worldPos) const;

  // 季节特性接口
  std::vector<std::string> getAvailableCrops(const std::string &season) const;
  std::vector<std::string> getSpecialEvents(const std::string &season) const;

  // NPC相关
  Vec2 getNPCSpawnPoint(const std::string &npcId) const;

  // 农场属性
  int getMaxCropSlots() const;
  bool hasWaterSource() const;
  bool isInitiallyUnlocked() const;

private:
  FarmMapManager() : map(nullptr) {}

  TMXTiledMap *map;
  FarmTypeConfig farmConfig;
  std::string currentFarmType;

  bool checkLayerProperty(const Vec2 &worldPos, const std::string &layerName,
                          const std::string &property) const;
  Vec2 worldToTileCoord(const Vec2 &worldPos) const;
};