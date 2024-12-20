#include "LandStates.h"
#include "Farmland.h"
#include "WeatherEffectFactory.h"
#include "CropFactory.h"
#include "WeatherEffects.h"

void WateredState::till(Farmland* land) {
	// �ѽ�ˮ�����ز��ܿ���
	CCLOG("Cannot till watered land!");
}

void WateredState::water(Farmland* land) {
	// ����ʪ�ȳ���ʱ��
	land->setWaterTimer(0.0f);
	CCLOG("Refreshed water!");
}

void WateredState::fertilize(Farmland* land) {
	land->setState(new WateredAndFertilizedState());
}

void WateredState::plant(Farmland* land, CropType cropType) {
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

	// �������ص�����ָ��
	land->setCrop(newCrop);

	CCLOG("Successfully planted a %s!", newCrop->getName().c_str());
}

void WateredState::update(Farmland* land, float dt) {
	updateCropGrowth(land);
	updateWaterTimer(land, dt);
	// ˮ�ֻ���ʱ������
	float timer = land->getWaterTimer();
	if (timer >= wateringDuration) {  // 24Сʱ��ˮ������
		transitionToTilledState(land);//ת�ؿ���״̬
		CCLOG("Land dried up, returning to tilled state");
	}
}

void WateredState::weatherEffect(Farmland* land, WeatherType weatherType) {
	auto effect = WeatherEffectFactory::createEffect(weatherType);
	if (effect) {
		effect->applyToWatered(land);
		effect->applyToCrop(land->getCrop());
	}
}
void WateredState::updateCropGrowth(Farmland* land) {
	// �ѿ����ҽ�ˮ����������
	Crop* crop = land->getCrop();
	if (crop) {
		crop->updateGrowthFromLand(true, false, 50.0f);  // ��������50%
		land->setCropGrowthRate(1.0f);  // �������������ٶ�Ϊ����
	}
}