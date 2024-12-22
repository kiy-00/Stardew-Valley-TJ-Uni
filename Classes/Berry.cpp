#include "Berry.h"

Berry* Berry::create() {
    Berry* berry = new (std::nothrow) Berry();
    if (berry && berry->init()) {
        berry->autorelease();
        return berry;
    }
    CC_SAFE_DELETE(berry);
    return nullptr;
}

bool Berry::init() {
    if (!Crop::init()) {
        return false;
    }

    cropType = "berry";
    totalGrowthTime = 72.0f; // 3��

    // ���ý�������������
    growthConditions.minTemp = 15.0f;
    growthConditions.maxTemp = 30.0f;
    growthConditions.preferredSeason = "Summer";
    growthConditions.waterNeed = 1.0f;

    return true;
}

void Berry::updateGrowth(float dt) {
    Crop::updateGrowth(dt);
    // �������е������߼�
}

bool Berry::checkGrowthConditions() {
    return isWatered && // ��Ҫˮ
        isFertilized; // ��Ҫ����
}
