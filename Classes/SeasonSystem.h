#ifndef __SEASON_SYSTEM_H__
#define __SEASON_SYSTEM_H__

#include "cocos2d.h"
#include "ISeasonState.h"

class ISeasonObserver;
class SeasonSystem : public cocos2d::Ref {
public:
    static SeasonSystem* getInstance();
    static void destroyInstance();

    // ���¼��ڣ�������Ϸʱ�䣩
    void update(float dt);

    
    SeasonType getCurrentSeason() const { return currentSeason->getSeasonName(); }
    void setCurrentSeason(ISeasonState* newSeason);
    ISeasonState* getSeasonState() { return currentSeason; }


    // ��ȡ��ǰ���ڵ�����(1-28)
    int getCurrentDay() const { return currentDay; }

    // ��ȡ��ǰ���
    int getCurrentYear() const { return currentYear; }


    //�۲������
    void addObserver(ISeasonObserver* observer);
    void removeObserver(ISeasonObserver* observer);
    void notifyObservers();

    // ����ת��
    void changeSeason();

private:
    SeasonSystem();
    ~SeasonSystem();

    static SeasonSystem* instance;//����ϵͳ����

    int currentDay;
    int currentYear;
    float accumulatedTime;  // �ۻ�����Ϸʱ��

    std::vector<ISeasonObserver*>m_observers;//�۲����б�

    ISeasonState* currentSeason;//��¼��ǰ����

};

#endif