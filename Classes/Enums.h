#pragma once
#ifndef __ENUMS_H__
#define __ENUMS_H__
enum class CropType {
	NONE,
	CORN,
	POTATO,
	TOMATO,
	WHEAT,
	CARROT
};

//��������
enum class ToolType {
	HOE,//��ͷ
	WATERING_CAN,//ˮ��
	FERTILIZER,//����
	PESTICIDE,//ɱ���
	SICKLE//����
};

//����
enum class SeasonType {
	SPRING,
	SUMMER,
	AUTUMN,
	WINTER
};

//����״̬
enum class LandState {
	NORMAL,//����
	TILLED,//����
	WATERED,//��ˮ
	FERTILIZED,//ʩ��
	WATERED_AND_FERTILIZED//��ˮ��ʩ��
};

//����״̬
enum class GrowthStage {
	SEED,       // ����
	SPROUT,     // ��ѿ
	GROWING,    // ������
	MATURE,      // ����
	DEAD
};

enum class CropQuality {
	NORMAL,     // ��ͨ
	SILVER,     // ����
	GOLD,        // ����
};

enum class WeatherType {
	SUNNY,      // ����
	SNOWY,     // ����
	RAINY,      // ����
	STORM,       // ������
};
enum class ThreatType {
	DROUGHT,
	DISEASE,
	NONE
};
#endif // __ENUMS_H__