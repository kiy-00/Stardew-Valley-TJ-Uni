#ifndef __PEPPER_H__
#define __PEPPER_H__

#include "Crop.h"

class Pepper : public Crop {
public:
    static Pepper* create(FarmlandTile* tile) {
        Pepper* pepper = new (std::nothrow) Pepper();
        if (pepper && pepper->init("pepper", tile)) {
            pepper->autorelease();
            pepper->initPepperProperties();
            return pepper;
        }
        CC_SAFE_DELETE(pepper);
        return nullptr;
    }

protected:
    virtual void initPepperProperties() {
        // ��������
        properties.growthRate = 1.3f;
        properties.waterNeed = 70.0f;
        properties.fertilityNeed = 50.0f;

        // ���������������ⷴӦ
        properties.weatherModifiers = {
            {"sunny", 1.4f},
            {"cloudy", 1.1f},
            {"rainy", 1.1f},
            {"snowy", 0.3f},
            {"foggy", 0.8f}
        };

        // �����ļ�������
        properties.seasonModifiers = {
            {"spring", 0.9f},
            {"summer", 1.4f},
            {"fall", 1.0f},
            {"winter", 0.1f}
        };
    }

    // ��д�ջ񷽷�
    virtual void harvest() override {
        if (!canHarvest()) return;

        // TODO: �������ջ��߼��������ұ���+1����
        // ������Ҫ����Ʒϵͳ�Խ�

        // �ջ��������ʧ
        this->removeFromParent();
    }
};

#endif // __PEPPER_H__
