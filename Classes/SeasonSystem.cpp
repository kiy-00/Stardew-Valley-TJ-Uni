// ʵ���ļ�
#include "SeasonSystem.h"
#include "ISeasonObserver.h"
#include "SeasonStates.h"

SeasonSystem* SeasonSystem::instance = nullptr;

SeasonSystem* SeasonSystem::getInstance() {
    if (instance == nullptr) {
        instance = new SeasonSystem();
        instance->init();
    }
    return instance;
}

void SeasonSystem::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

SeasonSystem::SeasonSystem()
    : currentSeason(new SummerState())
    , currentDay(1)
    , currentYear(1)
    , accumulatedTime(0.0f)
{
}

SeasonSystem::~SeasonSystem() {
    
}

void SeasonSystem::update(float dt) {
    accumulatedTime += dt;

    // ����1��Ϸ���� = 1��ʵ��
    // 1�� = 24��ϷСʱ = 1440��Ϸ����
    const float TIME_PER_DAY = 144.0f;

    if (accumulatedTime >= TIME_PER_DAY) {
        // ��������
        currentDay++;
        accumulatedTime -= TIME_PER_DAY;

        // ÿ����28��
        if (currentDay > 28) {
            currentDay = 1;
            changeSeason();
        }
    }
}

void SeasonSystem::setCurrentSeason(ISeasonState* newSeason) {
    if (currentSeason)
        delete currentSeason;
    currentSeason = newSeason;
}

void SeasonSystem::changeSeason() {
    // �л�����һ������
    currentSeason->changeToNextSeason(this);
    notifyObservers();

    // ��������һ���ѭ��
    if (currentSeason->getSeasonName()==SeasonType::SPRING) {
        currentYear++;
    }
 
}

void SeasonSystem::addObserver(ISeasonObserver* observer) {
    m_observers.push_back(observer);
}

void SeasonSystem::removeObserver(ISeasonObserver* observer) {
    m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
}

void SeasonSystem::notifyObservers() {
    // ֪ͨ���й۲���
    for (auto observer : m_observers) {
        observer->onSeasonChanged(this);
    }
}