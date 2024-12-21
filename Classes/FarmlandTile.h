#pragma once
// FarmlandTile.h
#pragma once
#include "cocos2d.h"

USING_NS_CC;

class FarmlandTile : public cocos2d::Node {
public:
    enum class State {
        EMPTY,          // δ����
        TILLED,         // �Ѹ���
        WATERED,        // �ѽ�ˮ
        FERTILIZED      // ��ʩ��
    };

    static FarmlandTile* create(const Vec2& position);

    bool init(const Vec2& position);

    // ״̬ת������
    void till();       // ����
    void water();      // ��ˮ
    void fertilize();  // ʩ��
    void resetState(); // ����״̬

    static void setTileSize(float size);

    // ��ȡ��ǰ״̬
    State getState() const { return currentState; }

    // ��ȡ����ʪ��/����������
    float getMoisture() const { return moisture; }
    float getFertility() const { return fertility; }

    // ����״̬������ˮ����ʱ����٣�
    void update(float dt);

private:
    State currentState;
    float moisture;    // ����ʪ�� 0-100
    float fertility;   // ���� 0-100

    static float tileSize;

    // ������ʾ
    void updateSprite();
};