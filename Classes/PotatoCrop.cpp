#include "PotatoCrop.h"

USING_NS_CC;


bool PotatoCrop::init() {

	initializeGrowthStages();

	return true;
}

void PotatoCrop::initializeGrowthStages() {
	m_stageTextures = {
		{GrowthStage::SEED, "crop/potato/potato_seed.png"},
		{GrowthStage::SPROUT, "crop/potato/potato_sprout.png"},
		{GrowthStage::GROWING, "crop/potato/potato_growing.png"},
		{GrowthStage::MATURE, "crop/potato/potato_mature.png"},
		{GrowthStage::DEAD,"crop/potato/potato_dead.png"}
	};

	// ���ó�ʼ��ͼ
	setTexture(m_stageTextures[GrowthStage::SEED]);
}