#include "WeatherEffects.h"

void SnowyEffect::applyToNormal(Farmland* land) {
	CCLOG("Snowy effect: No effect on unplowed land.");
}
void SnowyEffect::applyToTilled(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Snowy effect: Water froze, land state changed to Tilled.");
}
void SnowyEffect::applyToWatered(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Snowy effect: Water froze, land state changed to Tilled.");
}
void SnowyEffect::applyToFertilized(Farmland* land) {
	land->setFertilizeTimer(0.0f);  // ������ϼ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Snowy effect: Fertilizer invalidated, land state changed to Tilled.");
}
void SnowyEffect::applyToWateredAndFertilized(Farmland* land) {
	land->setWaterTimer(0.0f);  // ���ˮ�ּ�ʱ��
	land->setFertilizeTimer(0.0f);  // ������ϼ�ʱ��
	land->setState(new TilledState());  // תΪ����״̬
	CCLOG("Snowy effect: Water froze and fertilizer lost, land state changed to Tilled.");
}
void SnowyEffect::applyToCrop(Crop* crop) {
	if (crop != nullptr) {
		crop->setGrowthRate(0);  // ѩ�죬��������ֹͣ
	}
}