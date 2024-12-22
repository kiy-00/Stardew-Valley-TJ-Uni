#ifndef __TIME_EFFECT_MANAGER_H__
#define __TIME_EFFECT_MANAGER_H__

#include "cocos2d.h"
#include "TimeSeasonSystem.h"

USING_NS_CC;

class TimeEffectManager : public cocos2d::Node {
public:
    static TimeEffectManager* getInstance();
    virtual bool init() override;
    void initializeWithScene(Scene* scene);
    void updateTimeEffect(int hour);

    CREATE_FUNC(TimeEffectManager);

private:
    TimeEffectManager();
    ~TimeEffectManager();

    void clearCurrentEffect();
    void createDawnEffect();     // ���� (4:00-7:00)
    void createDayEffect();      // ���� (7:00-17:00)
    void createDuskEffect();     // �ƻ� (17:00-20:00)
    void createNightEffect();    // ҹ�� (20:00-4:00)

    static TimeEffectManager* instance;
    Scene* currentScene;
    LayerColor* currentEffect;

    // ʱ�䷶Χ����
    struct TimeRanges {
        static const int DAWN_START = 4;
        static const int DAWN_END = 7;
        static const int DAY_START = 7;
        static const int DAY_END = 17;
        static const int DUSK_START = 17;
        static const int DUSK_END = 20;
    };

    // Ч����������
    struct EffectParams {
        // ����Ч��
        static const GLubyte DAWN_ALPHA = 100;
        static const GLubyte DAWN_ALPHA_MIN = 80;
        static const Color3B DAWN_COLOR;  // (150, 170, 200)

        // �ƻ�Ч��
        static const GLubyte DUSK_ALPHA = 80;
        static const GLubyte DUSK_ALPHA_MIN = 60;
        static const GLubyte DUSK_ALPHA_MAX = 100;
        static const Color3B DUSK_COLOR;  // (255, 170, 120)

        // ҹ��Ч��
        static const GLubyte NIGHT_ALPHA = 150;
        static const GLubyte NIGHT_ALPHA_MIN = 140;
        static const GLubyte NIGHT_ALPHA_MAX = 160;
        static const Color3B NIGHT_COLOR;  // (20, 30, 60)
    };
};

#endif // __TIME_EFFECT_MANAGER_H__