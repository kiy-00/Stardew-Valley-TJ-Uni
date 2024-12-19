﻿// HelloWorldScene.h
#pragma once

#include "FarmMapManager.h"
#include "cocos2d.h"
#include "User.h"

class HelloWorldScene : public cocos2d::Scene {
public:
  static cocos2d::Scene *createScene();
  virtual bool init();
  CREATE_FUNC(HelloWorldScene);

private:
  // 地图相关
  cocos2d::TMXTiledMap *tmxMap;
  FarmMapManager *farmMapManager;
  std::string currentMapType;
  std::string currentSeason;

  User* player;

  // 初始化函数
  bool initMap();
  bool initPlayer();
  void setupTestMenu();
  void setupKeyboard();

  // 事件处理
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event *event);
  // void switchSeason(const std::string &season); // 注释掉这行

  // 更新函数
  void update(float dt) override;
 
  // 新增函数声明
  void initInventory();
  void setupMouse();
  void onMouseClick(cocos2d::EventMouse* event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
  void onSeasonChanged(const std::string &newSeason);

  // 季节测试相关
  cocos2d::Label* seasonLabel;
  const std::vector<std::string> seasons{"spring", "summer", "fall", "winter"};
  int currentSeasonIndex;
  void setupSeasonTest();
  void switchToNextSeason(float dt);
  void updateSeasonLabel(const std::string& season);
};