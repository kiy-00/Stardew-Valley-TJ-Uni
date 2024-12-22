#ifndef __CARROT_H__
#define __CARROT_H__

#include "Crop.h"

class Carrot : public Crop {
public:
    static Carrot* create(FarmlandTile* tile) {
        Carrot* carrot = new (std::nothrow) Carrot();
        if (carrot && carrot->init("carrot", tile)) {
            carrot->autorelease();
            carrot->initCarrotProperties();
            return carrot;
        }
        CC_SAFE_DELETE(carrot);
        return nullptr;
    }

protected:
    virtual void initCarrotProperties() {
        // ���ܲ�����
        properties.growthRate = 1.0f;
        properties.waterNeed = 50.0f;
        properties.fertilityNeed = 30.0f;

        // ���ܲ������������ⷴӦ
        properties.weatherModifiers = {
            {"sunny", 1.2f},
            {"cloudy", 1.0f},
            {"rainy", 1.3f},
            {"snowy", 0.2f},
            {"foggy", 0.8f}
        };

        // ���ܲ��ļ�������
        properties.seasonModifiers = {
            {"spring", 1.2f},
            {"summer", 1.0f},
            {"fall", 0.8f},
            {"winter", 0.3f}
        };
    }

    // ��д�ջ񷽷�
    virtual void harvest() override {
        if (!canHarvest()) return;

        // TODO: ���ܲ����ջ��߼��������ұ���+1���ܲ�
        // ������Ҫ����Ʒϵͳ�Խ�

        // �ջ��������ʧ
        this->removeFromParent();
    }
};

#endif // __CARROT_H__
