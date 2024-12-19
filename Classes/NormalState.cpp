#include "LandStates.h"
#include "Farmland.h"
#include "WeatherEffectFactory.h"
#include "WeatherEffects.h"

void NormalState::till(Farmland* land) {
	// ��ͨ״̬���Կ���
	land->setState(new TilledState());
}

void NormalState::water(Farmland* land) {
	// ��ͨ״̬����ֱ�ӽ�ˮ
	CCLOG("Cannot water untilled land!");
}

void NormalState::fertilize(Farmland* land) {
	//��ͨ״̬����ʩ��
	CCLOG("Cannot fertilize untilled land!");
}

void NormalState::plant(Farmland* land, CropType cropType) {
	// ��ͨ״̬������ֲ
	CCLOG("Cannot plant in untilled land!");
}
void NormalState::update(Farmland* land, float dt) {
	// ��ͨ����״̬�������
	CCLOG("Normal state update");
}
void NormalState::weatherEffect(Farmland* land, WeatherType weatherType) {
	auto effect = WeatherEffectFactory::createEffect(weatherType);
	if (effect) {
		effect->applyToNormal(land);
		effect->applyToCrop(land->getCrop());
	}
}

void NormalState::updateCropGrowth(Farmland* land) {
	;
}