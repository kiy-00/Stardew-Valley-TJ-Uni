﻿#pragma once
#include "FarmMapManager.h"
#include "FarmInteractionManager.h"
#include "TimeSeasonSystem.h"  // 添加时间季节系统头文件
#include "cocos2d.h"
#include "User.h"
#include "Inventory.h"
#include "ToolItem.h"

class FarmScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(const std::string& mapType);
    bool init(const std::string& mapType);

    static FarmScene* create(const std::string& mapType) {
        FarmScene* scene = new (std::nothrow) FarmScene();
        if (scene && scene->init(mapType)) {
            scene->autorelease();
            return scene;
        }
        CC_SAFE_DELETE(scene);
        return nullptr;
    }

private:
    // 地图相关
    cocos2d::TMXTiledMap* tmxMap;
    FarmMapManager* farmMapManager;
    std::string currentMapType;
    User* player;

    // 交互管理器
    FarmInteractionManager* interactionManager;

    // 时间季节系统相关
    TimeSeasonSystem* timeSystem;
    cocos2d::Label* timeSeasonLabel;  // 显示时间和季节的标签
    void updateTimeSeasonLabel();     // 更新时间显示
    void onSeasonChanged(const std::string& newSeason);

    // 初始化函数
    bool initMap();
    bool initPlayer();
    void setupKeyboard();
    void initInventory();
    void setupMouse();

    // 事件处理
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseClick(cocos2d::EventMouse* event);

    // 更新函数
    void update(float dt) override;

    // 背包系统相关
    bool isInventoryInitialized = false;
};