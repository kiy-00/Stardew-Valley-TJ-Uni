// FarmlandManager.cpp
#include "FarmlandManager.h"

FarmlandManager* FarmlandManager::instance = nullptr;



// FarmlandManager.cpp
bool FarmlandManager::init(cocos2d::Scene* scene, const std::vector<Vec2>& farmablePositions, int zOrder) {
    if (scene == nullptr) {
        CCLOG("map does not exists!");
        return false;
    }
    this->farmlandLayer = Layer::create();
    scene->addChild(farmlandLayer, zOrder);  // ʹ�ô���� zOrder
    this->farmablePositions = farmablePositions;

    // �������пɸ���λ�õ�FarmlandTile
    for (const auto& pos : farmablePositions) {
        auto tile = FarmlandTile::create(pos);
        if (tile) {
            farmlandLayer->addChild(tile);
            farmlands[positionToString(pos)] = tile;
        }
    }

    // �������µ����������� FarmlandManager::update
    farmlandLayer->schedule([this](float dt) {
        this->update(dt);
        }, 1.0f, "farmland_update");

    return true;
}
void FarmlandManager::handleToolAction(const std::string& toolType, const Vec2& position, int direction) {
    auto tile = getFarmlandAt(position);
    if (!tile) return;

    if (toolType == "hoe") {
        tile->till();
    }
    else if (toolType == "wateringcan") {
        tile->water();
    }
    else if (toolType == "fertilizer") {
        tile->fertilize();
    }
}

FarmlandTile* FarmlandManager::getFarmlandAt(const Vec2& position) {
    auto it = farmlands.find(positionToString(position));
    return it != farmlands.end() ? it->second : nullptr;
}

bool FarmlandManager::isFarmable(const Vec2& position) const {
    return std::find(farmablePositions.begin(), farmablePositions.end(), position) != farmablePositions.end();
}

void FarmlandManager::update(float dt) {
    // �������и���״̬
    for (auto& pair : farmlands) {
        pair.second->update(dt);
    }
}