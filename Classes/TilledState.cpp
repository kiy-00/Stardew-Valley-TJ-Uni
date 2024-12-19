#include "LandStates.h"
#include "Farmland.h"
#include "WeatherEffectFactory.h"
#include "WeatherEffects.h"

void TilledState::till(Farmland* land) {
	// �ѿ��ѵ������ٴο�����Ч
	CCLOG("Land is already tilled!");
}

void TilledState::water(Farmland* land) {
	// ���ѵ����ؿ��Խ�ˮ
	land->setState(new WateredState());
}

void TilledState::fertilize(Farmland* land) {
	//���ѹ������ؿ���ʩ��
	land->setState(new FertilizedState());
}

void TilledState::plant(Farmland* land, CropType cropType) {
	// ���ѵ����ؿ�����ֲ���������Ƚ�ˮ
	CCLOG("Land is dry! Consider watering first.");
	// �������ѡ���Ƿ������ڸ���������ֲ
}

void TilledState::update(Farmland* land, float dt) {
	updateCropGrowth(land);
	CCLOG("Tilled state update");
}

void TilledState::weatherEffect(Farmland* land, WeatherType weatherType) {
	auto effect = WeatherEffectFactory::createEffect(weatherType);
	if (effect) {
		effect->applyToTilled(land);
		effect->applyToCrop(land->getCrop());
	}
}
void TilledState::updateCropGrowth(Farmland* land) {
	// �ѿ��ѵ�δ��ˮ����������
	Crop* crop = land->getCrop();
	if (crop) {
		crop->updateGrowthFromLand(false, false, 30.0f);  // ��������30%
		land->setCropGrowthRate(0.8f);  // �������������ٶ�Ϊ����
	}
}