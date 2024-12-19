#pragma once
#ifndef __TIME_SYSTEM_H__
#define __TIME_SYSTEM_H__

#include "cocos2d.h"
#include "Enums.h"
#include <vector>

class TimeSystem {
public:
	// ����ģʽ
	static TimeSystem* getInstance() {
		static TimeSystem instance;
		return &instance;
	}

	// ʱ���ȡ
	int getCurrentHour() const { return m_hour; }
	int getCurrentDay() const { return m_day; }
	Season getCurrentSeason() const { return m_currentSeason; }
	int getYear() const { return m_year; }

	// ʱ�����
	void update(float dt);

	// ʱ��ת��
	float getGameTimeScale() const { return m_gameTimeScale; }
	void setGameTimeScale(float scale) { m_gameTimeScale = scale; }

	// �¼�ע��
	using TimeCallback = std::function<void()>;
	void registerDayChangeCallback(const TimeCallback& callback);
	void registerSeasonChangeCallback(const TimeCallback& callback);

private:
	TimeSystem()
		: m_hour(6)  // ��Ϸ������6�㿪ʼ
		, m_day(1)
		, m_currentSeason(Season::SPRING)
		, m_year(1)
		, m_gameTimeScale(60.0f)  // 1��ʵ�� = 1��Ϸ����
		, m_timeAccumulator(0.0f)
	{}

	int m_hour;
	int m_day;
	Season m_currentSeason;
	int m_year;
	float m_gameTimeScale;
	float m_timeAccumulator;

	std::vector<TimeCallback> m_dayChangeCallbacks;
	std::vector<TimeCallback> m_seasonChangeCallbacks;

	void advanceHour();
	void advanceDay();
	void advanceSeason();
	void notifyDayChange();
	void notifySeasonChange();

	static const int HOURS_PER_DAY = 24;
	static const int DAYS_PER_SEASON = 28;
	static const int SEASONS_PER_YEAR = 4;
};

#endif // __TIME_SYSTEM_H__