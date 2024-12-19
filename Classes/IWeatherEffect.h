#pragma once
#ifndef __I_WEATHER_EFFECT_H__
#define __I_WEATHER_EFFECT_H__

#include "WeatherEffectFactory.h"
#include "Farmland.h"
#include "LandStates.h"
class WeatherEffect {
public:
    // ���� apply ��������������״ִ̬�в�ͬЧ��
    virtual void applyToNormal(Farmland* land) = 0;
    virtual void applyToTilled(Farmland* land) = 0;
    virtual void applyToWatered(Farmland* land) = 0;
    virtual void applyToFertilized(Farmland* land) = 0;
    virtual void applyToWateredAndFertilized(Farmland* land) = 0;
    virtual void applyToCrop(Crop* crop) = 0;
    // ���Լ����������״̬�Ĵ�����
    virtual ~WeatherEffect() = default;
};

#endif // !__WEATHER_EFFECT_H__
