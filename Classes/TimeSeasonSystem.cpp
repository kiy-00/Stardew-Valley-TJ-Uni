// TimeSeasonSystem.cpp
#include "TimeSeasonSystem.h"

// ��̬��Ա��ʼ��
TimeSeasonSystem* TimeSeasonSystem::instance = nullptr;
const std::vector<std::string> TimeSeasonSystem::SEASON_NAMES = { "spring", "summer", "fall", "winter" };

// ���캯��
TimeSeasonSystem::TimeSeasonSystem()
    : currentTime{ 1, 0, 1, 6, 0 }  // �ӵ�һ�괺����һ������6�㿪ʼ
    , timeScale(1.0f)
    , accumulatedTime(0.0f)
    , isRunning(false) {
}

// ��������
TimeSeasonSystem::~TimeSeasonSystem() {
    instance = nullptr;
}

// ������ȡ
TimeSeasonSystem* TimeSeasonSystem::getInstance() {
    if (!instance) {
        instance = new (std::nothrow) TimeSeasonSystem();
        if (instance && instance->init()) {
            instance->autorelease();
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

// ��ʼ��
bool TimeSeasonSystem::init() {
    if (!Node::init()) {
        return false;
    }

    loadFromUserDefault();

    // ��������
    this->scheduleUpdate();

    return true;
}

// ʱ����Ʒ���
void TimeSeasonSystem::startTime() {
    isRunning = true;
    this->resume();
}

void TimeSeasonSystem::pauseTime() {
    isRunning = false;
    this->pause();
}

void TimeSeasonSystem::resumeTime() {
    isRunning = true;
    this->resume();
}

// ʱ�����
void TimeSeasonSystem::update(float dt) {
    if (!isRunning) return;

    // ����ʱ������
    // ����1����ʵʱ�� = 1������Ϸʱ�� * timeScale
    float deltaMinutes = dt * 60.0f * timeScale;
    updateTime(deltaMinutes);
}

void TimeSeasonSystem::updateTime(float deltaMinutes) {
    GameTime previousTime = currentTime;

    // �ۼӷ��Ӳ������λ
    accumulatedTime += deltaMinutes;
    while (accumulatedTime >= 1.0f) {
        currentTime.minute++;
        accumulatedTime -= 1.0f;

        if (currentTime.minute >= MINUTES_PER_HOUR) {
            currentTime.minute = 0;
            currentTime.hour++;

            if (currentTime.hour >= HOURS_PER_DAY) {
                currentTime.hour = 0;
                advanceDay();
            }
        }
    }

    // ��鲢�����¼�
    checkAndNotifyChanges(previousTime);
}

void TimeSeasonSystem::advanceDay() {
    int previousDay = currentTime.day;
    currentTime.day++;

    if (currentTime.day > DAYS_PER_SEASON) {
        currentTime.day = 1;
        advanceSeason();
    }

    notifyDayChange(previousDay);
}

void TimeSeasonSystem::advanceSeason() {
    Season previousSeason = static_cast<Season>(currentTime.season);
    currentTime.season = (currentTime.season + 1) % 4;

    if (currentTime.season == 0) {  // �µ�һ��
        currentTime.year++;
    }

    notifySeasonChange(previousSeason);
}

// �¼�֪ͨ
void TimeSeasonSystem::notifySeasonChange(Season previousSeason) {
    SeasonChangeEvent event{
        previousSeason,
        static_cast<Season>(currentTime.season),
        currentTime.year,
        currentTime.day
    };

    for (const auto& callback : seasonChangeCallbacks) {
        callback.second(event);
    }

    // ����״̬
    saveToUserDefault();
}

void TimeSeasonSystem::notifyDayChange(int previousDay) {
    DayChangeEvent event{
        previousDay,
        currentTime.day,
        static_cast<Season>(currentTime.season),
        currentTime.year
    };

    for (const auto& callback : dayChangeCallbacks) {
        callback.second(event);
    }
}

// �¼�����ע��
void TimeSeasonSystem::addSeasonChangeListener(const std::string& name, const SeasonChangeCallback& callback) {
    seasonChangeCallbacks[name] = callback;
}

void TimeSeasonSystem::addDayChangeListener(const std::string& name, const DayChangeCallback& callback) {
    dayChangeCallbacks[name] = callback;
}

void TimeSeasonSystem::removeSeasonChangeListener(const std::string& name) {
    seasonChangeCallbacks.erase(name);
}

void TimeSeasonSystem::removeDayChangeListener(const std::string& name) {
    dayChangeCallbacks.erase(name);
}

// ʱ������
void TimeSeasonSystem::setTimeScale(float scale) {
    timeScale = std::max(0.0f, scale);  // ��ֹ����ʱ������
}

void TimeSeasonSystem::setTime(const GameTime& time) {
    GameTime previousTime = currentTime;
    currentTime = time;

    // ȷ����ֵ����Ч��Χ��
    currentTime.minute = std::clamp(currentTime.minute, 0, MINUTES_PER_HOUR - 1);
    currentTime.hour = std::clamp(currentTime.hour, 0, HOURS_PER_DAY - 1);
    currentTime.day = std::clamp(currentTime.day, 1, DAYS_PER_SEASON);
    currentTime.season = std::clamp(currentTime.season, 0, 3);
    currentTime.year = std::max(1, currentTime.year);

    // ��鲢������Ӧ�¼�
    checkAndNotifyChanges(previousTime);
}

// �浵���
void TimeSeasonSystem::saveToUserDefault() {
    auto ud = UserDefault::getInstance();
    ud->setIntegerForKey("game_year", currentTime.year);
    ud->setIntegerForKey("game_season", currentTime.season);
    ud->setIntegerForKey("game_day", currentTime.day);
    ud->setIntegerForKey("game_hour", currentTime.hour);
    ud->setIntegerForKey("game_minute", currentTime.minute);
    ud->setFloatForKey("game_time_scale", timeScale);
    ud->flush();
}

void TimeSeasonSystem::loadFromUserDefault() {
    auto ud = UserDefault::getInstance();
    currentTime.year = ud->getIntegerForKey("game_year", 1);
    currentTime.season = ud->getIntegerForKey("game_season", 0);
    currentTime.day = ud->getIntegerForKey("game_day", 1);
    currentTime.hour = ud->getIntegerForKey("game_hour", 6);
    currentTime.minute = ud->getIntegerForKey("game_minute", 0);
    timeScale = ud->getFloatForKey("game_time_scale", 1.0f);
}

// ��������
void TimeSeasonSystem::checkAndNotifyChanges(const GameTime& previousTime) {
    if (currentTime.season != previousTime.season) {
        notifySeasonChange(static_cast<Season>(previousTime.season));
    }
    else if (currentTime.day != previousTime.day) {
        notifyDayChange(previousTime.day);
    }
}

TimeSeasonSystem::Season TimeSeasonSystem::intToSeason(int seasonInt) {
    return static_cast<Season>(seasonInt % 4);
}

std::string TimeSeasonSystem::seasonToString(Season season) {
    return SEASON_NAMES[static_cast<int>(season)];
}

std::string TimeSeasonSystem::getCurrentSeasonString() const {
    return SEASON_NAMES[currentTime.season];
}