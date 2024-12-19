#include "WeatherSystem.h"
#include "Farmland.h"
#include "IWeatherObserver.h"

USING_NS_CC;

WeatherSystem* WeatherSystem::s_instance = nullptr;

WeatherSystem* WeatherSystem::getInstance() {
	if (!s_instance) {
		s_instance = new WeatherSystem();
	}
	return s_instance;
}

WeatherSystem::WeatherSystem()
	: m_currentWeather(new  SunnyWeatherState())
	, m_weatherTimer(0.0f)
	, m_weatherDuration(300.0f)  // 5���ӳ��Ըı�һ������
{
	m_observers = std::vector<IWeatherObserver*>();
}
WeatherSystem::~WeatherSystem() {

}

void WeatherSystem::update(float dt) {
	m_weatherTimer += dt;//��ӵ�ǰ����������ʱ��

	if (m_weatherTimer >= m_weatherDuration) {
		m_currentWeather->update(this);
		m_weatherTimer = 0.0f;
	}
}

void WeatherSystem::changeWeather(IWeatherState* newState) {
	if (m_currentWeather) {
		delete m_currentWeather;  // ɾ����״̬
	}
	m_currentWeather = newState;  // ������״̬
	notifyObservers();  // ֪ͨ���й۲���
}

void WeatherSystem::addObserver(IWeatherObserver* observer) {
	m_observers.push_back(observer);
}

void WeatherSystem::removeObserver(IWeatherObserver* observer) {
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
}

void WeatherSystem::notifyObservers() {
	// ֪ͨ���й۲���
	for (auto observer : m_observers) {
		observer->onWeatherChanged(m_currentWeather->getWeatherName());
	}
}