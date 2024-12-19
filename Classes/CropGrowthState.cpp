#include "Crop.h"
// ���������״̬��


void SeedState::advance(Crop* crop) {
	crop->setGrowthState(new SproutState());
	crop->updateAppearance();
}



void SproutState::advance(Crop* crop) {
	crop->setGrowthState(new GrowingState());
	crop->updateAppearance();
}



void GrowingState::advance(Crop* crop) {
	crop->setGrowthState(new MatureState());
	crop->finalizeQuality();
	crop->updateAppearance();
}



void MatureState::advance(Crop* crop) {
	// �Ѿ����죬���ٽ���״̬ת��
}
