#include "TimeSystem.h"

void TimeSystem::update(float dt) {
	// �ۻ���ʵʱ��
	m_timeAccumulator += dt;

	// ת��Ϊ��Ϸʱ��
	float gameMinutes = m_timeAccumulator * m_gameTimeScale;

	// ÿ60���Ӹ���һ��Сʱ
	while (gameMinutes >= 60.0f) {
		advanceHour();
		gameMinutes -= 60.0f;
		m_timeAccumulator -= 60.0f / m_gameTimeScale;
	}
}

void TimeSystem::advanceHour() {
	m_hour++;
	if (m_hour >= HOURS_PER_DAY) {
		m_hour = 0;
		advanceDay();
	}
}

void TimeSystem::advanceDay() {
	m_day++;
	if (m_day > DAYS_PER_SEASON) {
		m_day = 1;
		advanceSeason();
	}
	notifyDayChange();
}

void TimeSystem::advanceSeason() {
	m_currentSeason = static_cast<Season>((static_cast<int>(m_currentSeason) + 1) % SEASONS_PER_YEAR);
	if (m_currentSeason == Season::SPRING) {
		m_year++;
	}
	notifySeasonChange();
}

void TimeSystem::registerDayChangeCallback(const TimeCallback& callback) {
	m_dayChangeCallbacks.push_back(callback);
}

void TimeSystem::registerSeasonChangeCallback(const TimeCallback& callback) {
	m_seasonChangeCallbacks.push_back(callback);
}

void TimeSystem::notifyDayChange() {
	for (const auto& callback : m_dayChangeCallbacks) {
		callback();
	}
}

void TimeSystem::notifySeasonChange() {
	for (const auto& callback : m_seasonChangeCallbacks) {
		callback();
	}
}