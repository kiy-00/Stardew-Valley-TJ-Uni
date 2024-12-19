#include "Crop.h"
#include "CropGrowthState.h"
#include "CropQualityStates.h"
#include "IThreatState.h"
#include "ThreatStates.h"

USING_NS_CC;

Crop* Crop::create(const std::string& cropName) {
	CCLOG("Cannot create instance of abstract class Crop");
	return nullptr;
}

Crop::~Crop() {
	if (m_growthState) delete m_growthState;
	if (m_qualityState) delete m_qualityState;
}

bool Crop::init() {
	if (!Sprite::init()) {
		return false;
	}

	// ��ʼ����������
	m_growthProgress = 0.0f;
	m_daysToMature = getGrowthDays();

	// ��ʼ�������������
	m_growthRate = 1.0f;
	m_qualityPoints = 0.0f;
	m_health = 100.0f;

	// ��ʼ��״̬
	m_growthState = new SeedState();
	m_qualityState = new NormalQualityState();

	// ��ʼ�������׶���ͼ
	initializeGrowthStages();
	updateAppearance();

	return true;
}

void Crop::grow(float dt) {
	if (m_growthState->getStage() == GrowthStage::MATURE || isDead()) {
		return;
	}

	// ����ʵ��������
	float baseGrowth = dt / (m_daysToMature * 24.0f);
	float actualGrowth = baseGrowth * m_growthRate;
	m_growthProgress += actualGrowth;

	// ����Ƿ���Ҫ������һ�׶�
	if (m_growthProgress >= 1.0f) {
		advanceGrowthStage();
	}

	// ����Ʒ��
	updateQuality();
}

void Crop::updateGrowthFromLand(bool isWatered, bool isFertilized, float fertility) {
	// ��������
	float growthModifier = 1.0f;
	if (isWatered) growthModifier *= 1.5f;
	if (isFertilized) growthModifier *= 1.3f;
	m_growthRate = growthModifier;

	// ����Ʒ��
	m_qualityState->updateQualityPoints(this, isWatered, isFertilized, fertility);
}

void Crop::advanceGrowthStage() {
	m_growthProgress = 0.0f;
	m_growthState->advance(this);
}

void Crop::setGrowthState(ICropGrowthState* state) {
	if (m_growthState) delete m_growthState;
	m_growthState = state;
	updateAppearance();
}

void Crop::setQualityState(ICropQualityState* state) {
	if (m_qualityState) delete m_qualityState;
	m_qualityState = state;
}

void Crop::died() {
	m_health = 0.0f;
	auto it = m_stageTextures.find(GrowthStage::DEAD);
	setTexture(it->second);
}

void Crop::onDamage(float damage) {
	m_health = std::max(0.0f, m_health - damage);
	if (m_health < 50.0f) {
		m_growthRate *= 0.8f;
	}
	if (isDead()) {
		died();
		CCLOG("%s has died!", m_name.c_str());
	}
}

int Crop::getHarvestValue() const {
	// �����۸�
	int basePrice = getBaseValue();

	// Ʒ�ʼӳɱ�����Ʒ��״̬����
	float qualityMultiplier = m_qualityState->getPriceMultiplier();

	// �������｡���ȵ�Ӱ��
	float healthModifier = m_health / 100.0f;

	return static_cast<int>(basePrice * qualityMultiplier * healthModifier);
}

void Crop::updateQuality() {
	if (m_qualityPoints >= 100.0f) {
		setQualityState(new GoldQualityState());
	}
	else if (m_qualityPoints >= 50.0f) {
		setQualityState(new SilverQualityState());
	}
	else {
		setQualityState(new NormalQualityState());
	}
}

void Crop::finalizeQuality() {
	updateQuality();
	CCLOG("%s final quality: %d", m_name.c_str(), static_cast<int>(m_qualityState->getQuality()));
}

void Crop::updateAppearance() {
	auto it = m_stageTextures.find(m_growthState->getStage());
	if (it != m_stageTextures.end()) {
		setTexture(it->second);
	}
	// �������в��������ɫ
	if (m_threatState) {
		if (m_threatState->getType() == ThreatType::DROUGHT) {
			this->setColor(Color3B(255, 200, 150));  // ƫ��
		}
		else if (m_threatState->getType() == ThreatType::DISEASE) {
			this->setColor(Color3B(150, 255, 150));  // ƫ��
		}
	}
	else {
		this->setColor(Color3B::WHITE);  // �ָ�������ɫ
	}
}

void Crop::updateThreatTimer(float dt) {
	// ����������в
	if (m_threatState) {//�����ǰ������в������ø���в��update����������˺�
		m_threatState->update(this, dt);
	}
	else {//������ۼ�����״̬�ĳ���ʱ��
		m_threatTimer += dt;
		if (m_threatTimer >= THREAT_CHECK_INTERVAL) {
			checkNewThreats();
			m_threatTimer = 0;
		}
	}

}

void Crop::checkNewThreats() {

	// ���ݻ����������������������в
	float random = CCRANDOM_0_1();
	if (random < 0.1f) {  // 10%����
		setThreatState(new DroughtThreatState());
	}
	else if (random < 0.15f) {  // 5%����
		setThreatState(new DiseaseThreatState());
	}
}

void Crop::setThreatState(IThreatState* state) {
	if (m_threatState) {
		delete m_threatState;
	}
	m_threatState = state;
	// �������
	updateAppearance();
}

ThreatType Crop::getThreatType() {
	if (m_threatState)
		return m_threatState->getType();
	return ThreatType::NONE;
}
