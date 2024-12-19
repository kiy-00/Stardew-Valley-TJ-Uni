#include "WeatherEffects.h"

void StormEffect::applyToNormal(Farmland* land) {
	land->setState(new TilledState());
	CCLOG("Storm effect: Soil erosion, land state changed to Tilled.");
}
void StormEffect::applyToTilled(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Storm effect: Water evaporated, land state changed to Tilled.");
}
void StormEffect::applyToWatered(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Storm effect: Water evaporated and land is affected.");
}
void StormEffect::applyToFertilized(Farmland* land) {
	land->setFertilizeTimer(0.0f);  // ������ϼ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Storm effect: Fertilizer washed away, land state changed to Tilled.");
}
void StormEffect::applyToWateredAndFertilized(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setFertilizeTimer(0.0f);  // ������ϼ�ʱ��
	land->setState(new FertilizedState());  // תΪʩ��״̬
	CCLOG("Storm effect: Water evaporated and fertilizer washed away, land state changed to Fertilized.");
}
void StormEffect::applyToCrop(Crop* crop) {
	if (crop != nullptr) {
		crop->setGrowthRate(0.5f);  // �籩��������������
	}
}