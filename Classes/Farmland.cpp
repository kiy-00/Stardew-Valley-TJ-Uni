#include "Farmland.h"
#include "LandStates.h" 
#include "CropFactory.h"
#include "ISeasonState.h"
#include <vector>
USING_NS_CC;

/**
* @brief ����ũ�����ľ�̬��������
* @param pos ũ���������е�λ��
* @return �ɹ�����ũ�����ָ��,ʧ�ܷ���nullptr
*/
Farmland* Farmland::create(const Position& pos) {
	Farmland* farmland = new (std::nothrow) Farmland();
	if (farmland && farmland->init(pos)) {
		farmland->autorelease();
		return farmland;
	}
	CC_SAFE_DELETE(farmland);
	return nullptr;
}

/**
* @brief ��ʼ��ũ�����
* @param pos ũ���������е�λ��
* @return ��ʼ���ɹ�����true,ʧ�ܷ���false
* @details ���ó�ʼ״̬�������λ�õ�
*/
bool Farmland::init(const Position& pos) {

	if (!Sprite::initWithFile("farmland/normal.png")) {
		return false;
	}

	m_gridPosition = pos;
	m_currentState = new NormalState();
	m_currentCrop = nullptr;

	// ��ʼ��״̬
	m_waterTimer=0.0f;
	m_fertilizeTimer=0.0f;
	m_fertility = 0.0f;

	setPosition(pos.x * 32, pos.y * 32);

	return true;
}

// ������ʹ��
bool Farmland::handleTool(ITool* tool) {
	if (!tool) 
		return false;
	return tool->useTool(this);
}


/**
* @brief ��������״̬
* @param newState �µ�״̬����ָ��
* @details ɾ����״̬,������״̬,���������
*/
void Farmland::setState(ILandState* newState) {
	if (m_currentState) {
		delete m_currentState;
	}
	m_currentState = newState;
	updateAppearance();
}

/**
* @brief ��������
* @details ���õ�ǰ״̬��till���������Ѳ���
*/
void Farmland::till() {
	if (m_currentState) {
		m_currentState->till(this);
		return;
	}
}

/**
* @brief ��ˮ
* @details ���õ�ǰ״̬��water��������ˮ����
*/
void Farmland::water() {
	if (m_currentState) {
		m_currentState->water(this);
		//�����ˮ��ʱ�������������û���������ھ����ɺ�
		if (m_currentCrop && !m_currentCrop->isDead() && m_currentCrop->getThreatType() == ThreatType::DROUGHT)
			m_currentCrop->setThreatState(nullptr);//��������ǰ���ֺ�
		return;
	}
}


void Farmland::fertilize() {
	if (m_currentState) {
		m_currentState->fertilize(this);
		return;
	}
}
/**
* @brief ��ֲ����
* @param crop Ҫ��ֲ���������ָ��
* @details ���õ�ǰ״̬��plant����������ֲ����
*/
void Farmland::plant(CropType cropType) {
	if (m_currentState) {
		m_currentState->plant(this, cropType);
		return;
	}
}

bool Farmland::harvest() {
	if (!m_currentCrop || !m_currentCrop->isReadyToHarvest()) {
		CCLOG("Nothing to harvest!");
		return false;
	}

	// ��ȡ�����ֵ
	int value = m_currentCrop->getHarvestValue();
	// �Ƴ�����
	removeCrop();

	// TODO: ����ջ񶯻���Ч��
	CCLOG("Harvested crop! Value: %d", value);
	return true;
}

/**
* @brief ���º���,�̳���Sprite
* @param dt ʱ������
* @details ���õ�ǰ״̬��update��������״̬����
*/
void Farmland::update(float dt) {
	// ��������״̬
	m_currentState->update(this, dt);

	// ��������
	if (m_currentCrop != nullptr) {
		m_currentCrop->grow(dt);  // �����������������
		m_currentCrop->updateThreatTimer(dt);//����������ֺ��ۼ�ʱ��
	}
}

/**
* @brief ����ũ�����
* @details ���ݵ�ǰ����״̬(�Ƿ�ˮ���Ƿ񿪿�)���ö�Ӧ������
*/
void Farmland::updateAppearance() {
	setTexture(m_currentState->getTexturePath());
}


void Farmland::setCrop(Crop* newCrop) {
	m_currentCrop = newCrop;
}

void Farmland::removeCrop() {
	if (m_currentCrop) {
		m_currentCrop->removeFromParent();  // �ӳ����ڵ������Ƴ�
		delete m_currentCrop;
	}
	m_currentCrop = nullptr;
}

void Farmland::onWeatherChanged(const WeatherType weatherType) {
	m_currentState->weatherEffect(this,weatherType);
}

void Farmland::setCropGrowthRate(float growthRate) {
	m_currentCrop->setGrowthRate(1.0f);
}

void Farmland::onSeasonChanged(SeasonSystem* seasonSystem) {
	if(m_currentCrop)
		seasonSystem->getSeasonState()->canCropGrow(m_currentCrop);
}
