#include "Carrot.h"

Carrot* Carrot::create() {
    Carrot* carrot = new (std::nothrow) Carrot();
    if (carrot && carrot->init()) {
        carrot->autorelease();
        return carrot;
    }
    CC_SAFE_DELETE(carrot);
    return nullptr;
}

bool Carrot::init() {
    if (!Crop::init()) {
        return false;
    }

    cropType = "carrot";
    totalGrowthTime = 96.0f; // 4��

    // ���ú��ܲ�����������
    growthConditions.minTemp = 10.0f;
    growthConditions.maxTemp = 25.0f;
    growthConditions.preferredSeason = "Spring";
    growthConditions.waterNeed = 0.8f;

    return true;
}

void Crop::updateGrowth(float dt) {
    Crop::updateGrowth(dt);
    // ���ܲ����е������߼�
}

bool Carrot::checkGrowthConditions() {
    return isWatered; // ���ܲ�ֻ��Ҫˮ
}