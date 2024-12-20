#pragma once
#ifndef __WEATHER_SYSTEM_H__
#define __WEATHER_SYSTEM_H__

#include "cocos2d.h"
#include "Enums.h"
#include <vector>
#include "WeatherStates.h"

// ǰ������
class Farmland;
class IWeatherState;
class IWeatherObserver;

class WeatherSystem : public cocos2d::Node {
public:
	// ����ģʽ
	static WeatherSystem* getInstance();

	// ������������
	WeatherType getCurrentWeather() const { return m_currentWeather->getWeatherName(); }
	
	// �ۻ���������ʱ��
	void update(float dt);

	//�۲������
	void addObserver(IWeatherObserver* observer);
	void removeObserver(IWeatherObserver* observer);
	void notifyObservers();

	//����ת��
	void changeWeather(IWeatherState* newState);

private:
	WeatherSystem();
	~WeatherSystem();

	static WeatherSystem* s_instance;//����ϵͳ����


	float m_weatherTimer;		//��¼��ǰ���������˶೤ʱ��
	float m_weatherDuration;    //ָ����ó��Ըı�һ������

	std::vector<IWeatherObserver*> m_observers;  // �۲����б�

	IWeatherState* m_currentWeather;//��¼��ǰ����
};

#endif // __WEATHER_SYSTEM_H__