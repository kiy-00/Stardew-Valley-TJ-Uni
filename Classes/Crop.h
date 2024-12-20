#pragma once
#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"
#include "Position.h"
#include "Enums.h"
#include "CropGrowthState.h"
#include "ICropQualityState.h"

USING_NS_CC;
class IThreatState;
class Crop : public Sprite {
public:
	Crop() {};
	static Crop* create(const std::string& cropName);
	virtual bool init() override;
	virtual ~Crop();

	// �������Է�����
	virtual CropType getCropType()const = 0;//��ȡ��������
	virtual std::vector<SeasonType> getSupportSeason() const = 0;//��ȡ���������ļ���
	virtual int getGrowthDays() const = 0;
	GrowthStage getGrowthStage() const { return m_growthState->getStage(); }
	CropQuality getQuality() const { return m_qualityState->getQuality(); }
	int getDaysToMature() const { return m_daysToMature; }//��ȡ�ɳ�����������
	bool isReadyToHarvest() const { return m_growthState->getStage() == GrowthStage::MATURE; }

	// �������
	virtual void grow(float dt);
	void setGrowthRate(float rate) { m_growthRate = rate; }//������������
	float getGrowthRate() const { return m_growthRate; }
	void updateGrowthFromLand(bool isWatered, bool isFertilized, float fertility);
	void advanceGrowthStage();
	void finalizeQuality();
	virtual void updateAppearance();

	// ״̬���
	void setGrowthState(ICropGrowthState* state);
	void setQualityState(ICropQualityState* newState);
	void setQualityPoints(float newPoints) { m_qualityPoints = newPoints; }
	float getQualityPoints() { return m_qualityPoints; }

	//�ֺ����
	void updateThreatTimer(float dt);
	void checkNewThreats();//��������ֺ���в
	void setThreatState(IThreatState* state);
	ThreatType getThreatType();

	// ����״̬
	float getHealth() const { return m_health; }
	bool isDead() const { return m_health <= 0; }
	void died();
	void onDamage(float damage);

	// �ջ����
	virtual int getBaseValue() const = 0;
	virtual int getHarvestValue() const = 0;

protected:
	// ��������
	float m_growthProgress;     // ��ǰ�׶ε��������� (0-1)��ÿ�ε�1�ͽ�����һ�׶�
	int m_daysToMature;         // ������������
	IThreatState* m_threatState = nullptr;//��¼��ǰ��в״̬
	float m_threatTimer = 0;//����״̬����ʱ��
	const float THREAT_CHECK_INTERVAL = 5.0f;  //�����������в��ʱ����
	std::vector<SeasonType> m_growableSeasons;


	// ״̬���
	ICropGrowthState* m_growthState;
	ICropQualityState* m_qualityState;

	// �������
	float m_growthRate;         // �����ٶ�����
	float m_qualityPoints;      // Ʒ�ʻ��۵���
	float m_health;             // ֲ�｡���� (0-100)

	// ��ͼ���
	std::map<GrowthStage, std::string> m_stageTextures;
	
	virtual void initializeGrowthStages() = 0;

private:
	void updateQuality();
};

#endif // __CROP_H__