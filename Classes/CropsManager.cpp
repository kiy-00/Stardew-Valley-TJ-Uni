#include "CropsManager.h"
#include "FarmMapManager.h"
#include "FarmConfigManager.h"
#include "FarmlandManager.h"

CropsManager* CropsManager::instance = nullptr;

CropsManager* CropsManager::getInstance() {
    if (!instance) {
        instance = new CropsManager();
    }
    return instance;
}

bool CropsManager::init(cocos2d::Scene* scene, const std::string& farmType,
    const std::string& season, int zOrder) {
    if (scene == nullptr) {
        CCLOG("Scene does not exist!");
        return false;
    }

    currentFarmType = farmType;
    currentSeason = season;

    this->cropsLayer = Layer::create();
    scene->addChild(cropsLayer, zOrder);

    cropsLayer->schedule([this](float dt) {
        this->update(dt);
        }, 1.0f, "crops_update");

    return true;
}

void CropsManager::plantCrop(const std::string& cropType, const Vec2& worldPos) {
    CCLOG("CropsManager::plantCrop - Start");
    CCLOG("Crop Type: %s, World Position: (%.1f, %.1f)",
        cropType.c_str(), worldPos.x, worldPos.y);

    if (!checkPlantingConditions(cropType, worldPos)) {
        return;
    }

    Vec2 tilePos = FarmMapManager::getInstance()->worldToTileCoord(worldPos);
    std::string key = positionToString(tilePos);

    // ����������
    auto crop = Crop::create(cropType);
    if (crop) {
        crop->setPosition(worldPos);
        cropsLayer->addChild(crop);
        crops[key] = crop;

        CCLOG("Successfully planted %s at position: %s",
            cropType.c_str(), key.c_str());
    }

    CCLOG("CropsManager::plantCrop - End");
}

bool CropsManager::checkPlantingConditions(const std::string& cropType, const Vec2& worldPos) const {
    // 1. ����Ƿ��ڵ�ǰũ���ͼ��ڿ�����ֲ������
    if (!isCropAvailable(cropType)) {
        CCLOG("This crop cannot be planted in current farm/season!");
        return false;
    }

    // 2. ���λ���Ƿ�ɸ���
    if (!FarmMapManager::getInstance()->isArable(worldPos)) {
        CCLOG("Position is not arable!");
        return false;
    }

    // 3. ����Ƿ���������
    Vec2 tilePos = FarmMapManager::getInstance()->worldToTileCoord(worldPos);
    std::string key = CropsManager::positionToString(tilePos);
    if (crops.find(key) != crops.end()) {
        CCLOG("Crop already exists at this position!");
        return false;
    }

    // 4. ����Ƿ����ѿ��ѵ�����
    auto farmland = FarmlandManager::getInstance()->getFarmlandAt(tilePos);
    if (!farmland || !farmland->isTilled()) {
        CCLOG("No tilled farmland at this position!");
        return false;
    }

    return true;
}

void CropsManager::harvestCrop(const Vec2& worldPos) {
    CCLOG("CropsManager::harvestCrop - Start");

    auto crop = getCropAt(worldPos);
    if (!crop || !crop->isFullyGrown()) {
        CCLOG("No harvestable crop at this position!");
        return;
    }

    // ����������ջ��߼���������ӵ���ұ���

    // �Ƴ�����
    removeCrop(worldPos);

    CCLOG("CropsManager::harvestCrop - End");
}

void CropsManager::removeCrop(const Vec2& worldPos) {
    Vec2 tilePos = FarmMapManager::getInstance()->worldToTileCoord(worldPos);
    std::string key = positionToString(tilePos);

    auto it = crops.find(key);
    if (it != crops.end()) {
        it->second->removeFromParent();
        crops.erase(it);
    }
}

Crop* CropsManager::getCropAt(const Vec2& worldPos) {
    Vec2 tilePos = FarmMapManager::getInstance()->worldToTileCoord(worldPos);
    std::string key = positionToString(tilePos);
    auto it = crops.find(key);
    return (it != crops.end()) ? it->second : nullptr;
}

bool CropsManager::isCropAvailable(const std::string& cropType) const {
    auto availableCrops = FarmConfigManager::getInstance()->getAvailableCrops(currentFarmType, currentSeason);
    return std::find(availableCrops.begin(), availableCrops.end(), cropType) != availableCrops.end();
}

void CropsManager::handleToolAction(const std::string& toolType, const Vec2& worldPos, int direction) {
    CCLOG("CropsManager::handleToolAction - Start");

    auto crop = getCropAt(worldPos);
    if (!crop) {
        CCLOG("No crop found at this position!");
        return;
    }

    if (toolType == "Sickle") {
        if (crop->isFullyGrown()) {
            harvestCrop(worldPos);
        }
    }
    else if (toolType == "Wateringcan") {
        crop->water();
    }
    else if (toolType == "Fertilizer") {
        crop->fertilize();
    }

    CCLOG("CropsManager::handleToolAction - End");
}

void CropsManager::handleWeatherChange(float temperature, const std::string& weather) {
    for (auto& pair : crops) {
        pair.second->handleWeatherChange(temperature, weather);
    }
}

void CropsManager::handleSeasonChange(const std::string& season) {
    currentSeason = season;

    // ���ÿ���������¼����Ƿ���Լ�������
    std::vector<Vec2> cropsToRemove;

    for (const auto& pair : crops) {
        auto cropType = pair.second->getCropType();
        if (!isCropAvailable(cropType)) {
            // ����������¼��ڲ�����������¼��λ���Ա��Ƴ�
            Vec2 pos = pair.second->getPosition();
            cropsToRemove.push_back(pos);
        }

        // ֪ͨ���＾�ڱ仯
        pair.second->handleSeasonChange(season);
    }

    // �Ƴ��������¼�������������
    for (const auto& pos : cropsToRemove) {
        removeCrop(pos);
    }
}

std::vector<std::string> CropsManager::getAvailableCrops() const {
    return FarmConfigManager::getInstance()->getAvailableCrops(currentFarmType, currentSeason);
}

std::string CropsManager::positionToString(const Vec2& pos) {
    return StringUtils::format("%.0f,%.0f", std::round(pos.x), std::round(pos.y));
}

void CropsManager::update(float dt) {
    // ����ĸ����Ѿ��ڸ��ԵĶ�ʱ���д���
    // ���������Ӷ���Ĺ����߼�
}