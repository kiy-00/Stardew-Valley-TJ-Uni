#include "WeatherStates.h"
#include "Farmland.h"


//����
void SunnyWeatherState::update(WeatherSystem* weatherSystem) {
    // 10% ���ʱ�Ϊ����
    if (rand() % 100 < 10) {
        weatherSystem->changeWeather(new RainyWeatherState());
    }
    // 5% ���ʱ�Ϊ�籩
    else if (rand() % 100 < 5) {
        weatherSystem->changeWeather(new StormWeatherState());
    }
}


//����
void RainyWeatherState::update(WeatherSystem* weatherSystem) {
    // 20% ���ʱ�Ϊ����
    if (rand() % 100 < 20) {
        weatherSystem->changeWeather(new SunnyWeatherState());
    }
    // 10% ���ʱ�Ϊ�籩
    else if (rand() % 100 < 10) {
        weatherSystem->changeWeather(new StormWeatherState());
    }
}


//�籩
// StormWeatherState.cpp
void StormWeatherState::update(WeatherSystem* weatherSystem) {
    // 30% ���ʱ�Ϊ����
    if (rand() % 100 < 30) {
        weatherSystem->changeWeather(new RainyWeatherState());
    }
    // 5% ���ʱ�Ϊ����
    else if (rand() % 100 < 5) {
        weatherSystem->changeWeather(new SunnyWeatherState());
    }
}


//ѩ��
void SnowyWeatherState::update(WeatherSystem* weatherSystem) {
    if (rand() % 100 < 50) {
        weatherSystem->changeWeather(new SunnyWeatherState());  // 50% �ĸ��ʱ�Ϊ����
    }
    else if (rand() % 100 < 30) {
        weatherSystem->changeWeather(new RainyWeatherState());  // 30% �ĸ��ʱ�Ϊ����
    }
    else {
        weatherSystem->changeWeather(new StormWeatherState());  // 20% �ĸ��ʱ�Ϊ�籩
    }
}
