#ifndef __WHEAT_H__
#define __WHEAT_H__

#include "Crop.h"

class Wheat : public Crop {
public:
    static Wheat* create(FarmlandTile* tile) {
        Wheat* wheat = new (std::nothrow) Wheat();
        if (wheat && wheat->init("wheat", tile)) {
            wheat->autorelease();
            wheat->initWheatProperties();
            return wheat;
        }
        CC_SAFE_DELETE(wheat);
        return nullptr;
    }

protected:
    virtual void initWheatProperties() {
        // С������
        properties.growthRate = 1.1f;
        properties.waterNeed = 40.0f;
        properties.fertilityNeed = 35.0f;

        // С������������ⷴӦ
        properties.weatherModifiers = {
            {"sunny", 1.3f},
            {"cloudy", 1.1f},
            {"rainy", 1.0f},
            {"snowy", 0.5f},
            {"foggy", 0.9f}
        };

        // С��ļ�������
        properties.seasonModifiers = {
            {"spring", 1.0f},
            {"summer", 1.3f},
            {"fall", 1.1f},
            {"winter", 0.3f}
        };
    }

    // ��д�ջ񷽷�
    virtual void harvest() override {
        if (!canHarvest()) return;

        // TODO: С����ջ��߼��������ұ���+��������
        // ������Ҫ����Ʒϵͳ�Խ�

        // �ջ��������ʧ
        this->removeFromParent();
    }
};

#endif // __WHEAT_H__
