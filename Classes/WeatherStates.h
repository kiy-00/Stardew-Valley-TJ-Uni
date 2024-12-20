#pragma once
#ifndef __WEATHER_STATES_H__
#define __WEATHER_STATES_H__
#include "IWeatherState.h"
#include "Enums.h"

//����
class SunnyWeatherState : public IWeatherState {
public:
    void update(WeatherSystem* weatherSystem) override;
    const WeatherType getWeatherName() const override { return WeatherType::SUNNY; }
};

//����
class RainyWeatherState : public IWeatherState {
public:
    void update(WeatherSystem* weatherSystem) override;
    const WeatherType getWeatherName() const override { return WeatherType::RAINY; }


};

//�籩
class StormWeatherState : public IWeatherState {
public:
    void update(WeatherSystem* weatherSystem) override;
    const WeatherType getWeatherName() const override { return WeatherType::STORM; }
};

//ѩ��
class SnowyWeatherState : public IWeatherState {
public:
    void update(WeatherSystem* weatherSystem) override;
    const WeatherType getWeatherName() const override { return WeatherType::SNOWY; }
};
#endif // !__WEATHER_STATES__
