#include "CropQualityStates.h"
#include "Crop.h"
void NormalQualityState::updateQualityPoints(Crop* crop, bool isWatered, bool isFertilized, float fertility) {
    float points = crop->getQualityPoints();
    // ��������
    float baseIncrease = 1.0f;
    // �����ӳ�
    if (isWatered && isFertilized) {
        baseIncrease *= 2.0f;
    }
    else if (isWatered) {
        baseIncrease *= 1.5f;
    }
    // ���ط����ӳ�
    baseIncrease *= (1.0f + fertility / 100.0f);

    points += baseIncrease;
    crop->setQualityPoints(points);

    // ����Ƿ�ﵽ��������
    if (points >= 150) {
        crop->setQualityState(new SilverQualityState());
    }
}
void SilverQualityState::updateQualityPoints(Crop* crop, bool isWatered, bool isFertilized, float fertility) {
    float points = crop->getQualityPoints();
    // ���ʵȼ������������Ը�
    float baseIncrease = 1.5f;

    // �����ӳ�
    if (isWatered && isFertilized) {
        baseIncrease *= 2.2f;  // �Ը�����ͨƷ�ʵļӳ�
    }
    else if (isWatered) {
        baseIncrease *= 1.7f;
    }

    // ���ط����ӳɣ����ʵȼ��Է���������
    baseIncrease *= (1.0f + fertility / 90.0f);

    points += baseIncrease;
    crop->setQualityPoints(points);

    // ����Ƿ�ﵽ�ƽ�Ʒ��
    if (points >= 300) {
        crop->setQualityState(new GoldQualityState());
    }
}
void GoldQualityState::updateQualityPoints(Crop* crop, bool isWatered, bool isFertilized, float fertility) {
    float points = crop->getQualityPoints();
    // �ƽ�ȼ��������������
    float baseIncrease = 2.0f;

    // �����ӳ�
    if (isWatered && isFertilized) {
        baseIncrease *= 2.5f;  // ���Ʒ�ʵ����ӳ�
    }
    else if (isWatered) {
        baseIncrease *= 2.0f;
    }

    // ���ط����ӳɣ��ƽ�ȼ��Է���������
    baseIncrease *= (1.0f + fertility / 80.0f);

    points += baseIncrease;

    // �ƽ�Ʒ��������
    points = std::min(points, 500.0f);

    crop->setQualityPoints(points);
    // �ƽ�Ʒ������ߵȼ�������Ҫ�������
}