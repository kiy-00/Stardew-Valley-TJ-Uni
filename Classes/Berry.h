#ifndef __BERRY_H__
#define __BERRY_H__

#include "Crop.h"

class Berry : public Crop {
public:
    static Berry* create(FarmlandTile* tile) {
        Berry* berry = new (std::nothrow) Berry();
        if (berry && berry->init("berry", tile)) {
            berry->autorelease();
            berry->initBerryProperties();
            return berry;
        }
        CC_SAFE_DELETE(berry);
        return nullptr;
    }

protected:
    virtual void initBerryProperties() {
        // ݮ����������
        properties.growthRate = 1.2f;
        properties.waterNeed = 60.0f;
        properties.fertilityNeed = 40.0f;

        // ݮ�������������ⷴӦ
        properties.weatherModifiers = {
            {"sunny", 1.3f},   // ���������������
            {"cloudy", 1.0f},
            {"rainy", 1.2f},   // ϲ����ˮ
            {"snowy", 0.3f},   // �����ܺ�
            {"foggy", 0.9f}
        };

        // ݮ���ļ�������
        properties.seasonModifiers = {
            {"spring", 1.1f},
            {"summer", 1.4f},  // �ļ��������
            {"fall", 0.9f},
            {"winter", 0.4f}   // ���������ܲ�
        };
    }

    // ��д�ջ񷽷�
    virtual void harvest() override {
        if (!canHarvest()) return;

        // TODO: ����ջ���Ʒ����ұ���
        // ������Ҫ����Ʒϵͳ�Խ�

        // �ջ��������ʧ
        this->removeFromParent();
    }
};

#endif // __BERRY_H__