#include "LandStates.h"
#include "Farmland.h"
#include "WeatherEffectFactory.h"
#include "CropFactory.h"
#include "WeatherEffects.h"

void WateredAndFertilizedState::till(Farmland* land) {
	CCLOG("Cannot till watered and fertilized land!");
}

void WateredAndFertilizedState::water(Farmland* land) {
	CCLOG("Land is already watered!");
}

void WateredAndFertilizedState::fertilize(Farmland* land) {
	CCLOG("Land is already fertilized!");
}

void WateredAndFertilizedState::plant(Farmland* land, CropType cropType) {
	// ��������Ƿ���������
	if (land->hasCrop()) {
		CCLOG("Cannot plant: There is already a crop in this land!");
		return;
	}

	// ʹ�ù�������ָ�����͵�����
	Crop* newCrop = CropFactory::getInstance()->createCrop(cropType);
	if (!newCrop) {
		CCLOG("Failed to create crop!");
		return;
	}
	if (!canGrowInSeason(newCrop)) {
		CCLOG("Can't grow the crop in current season.");
		return;
	}


	// ��ȡ���صĳߴ��λ����Ϣ
	cocos2d::Size landSize = land->getContentSize();

	// ���������Ϊ���ص��ӽڵ�
	land->addChild(dynamic_cast<cocos2d::Node*>(newCrop));
	newCrop->setPosition(cocos2d::Vec2(landSize.width / 2, landSize.height / 2));

	// ��Ϊ��������״̬�����Ը�����һЩ��ʼ�ɳ��ӳ�
	newCrop->updateGrowthFromLand(true, true, 100.0f); // ��������������������ļӳ�

	// �������ص�����ָ��
	land->setCrop(newCrop);

	CCLOG("Successfully planted a %s in ideal conditions!", newCrop->getName().c_str());
}

void WateredAndFertilizedState::update(Farmland* land, float dt) {
	updateCropGrowth(land);
	updateWaterTimer(land, dt);
	updateFertilizeTimer(land, dt);

	float waterTimer = land->getWaterTimer();
	float fertilizeTimer = land->getFertilizeTimer();
	bool waterDepleted = waterTimer >= wateringDuration;
	bool fertilizerDepleted = fertilizeTimer >= fertilizingDuration;

	if (waterDepleted && fertilizerDepleted) {
		transitionToTilledState(land);
	}
	else if (waterDepleted) {
		transitionToFertilizedState(land);
	}
	else if (fertilizerDepleted) {
		transitionToWateredState(land);
	}

	CCLOG("WateredAndFertilized state update");
}

void WateredAndFertilizedState::weatherEffect(Farmland* land, WeatherType weatherType) {
	auto effect = WeatherEffectFactory::createEffect(weatherType);
	if (effect) {
		effect->applyToWateredAndFertilized(land);
		effect->applyToCrop(land->getCrop());
	}
}
void WateredAndFertilizedState::updateCropGrowth(Farmland* land) {
	// ������״̬����������
	Crop* crop = land->getCrop();
	if (crop) {
		crop->updateGrowthFromLand(true, true, 100.0f);  // ��߷���100%
		land->setCropGrowthRate(1.5f);  // �������������ٶ�Ϊ����
	}
}