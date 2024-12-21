#pragma once
// FarmlandManager.h
#pragma once
#include "cocos2d.h"
#include "FarmlandTile.h"
#include <unordered_map>
#include<vector>

USING_NS_CC;

class FarmlandManager {
public:
    static FarmlandManager* getInstance() {
        if (!instance) {
            instance = new FarmlandManager();
        }
        return instance;
    }

    // FarmlandManager.h
    bool init(cocos2d::Scene* scene, const std::vector<Vec2>& farmablePositions, int zOrder);

    // ������ʹ���¼�
    void handleToolAction(const std::string& toolType, const Vec2& position, int direction);

    // ��ȡָ��λ�õĸ���״̬
    FarmlandTile* getFarmlandAt(const Vec2& position);

    std::vector<FarmlandTile*> getallFarmlands()const
    {

        std::vector<FarmlandTile*>lands;
        for (auto& land : farmlands)
        {
            lands.push_back(land.second);
        }

        return lands;
    }

    // ���λ���Ƿ���Ը���
    bool isFarmable(const Vec2& position) const;

    // �������и���״̬�������ڶ�ʱ���£�
    void update(float dt);

private:
    FarmlandManager() {}
    static FarmlandManager* instance;

    cocos2d::Layer* farmlandLayer;
    std::unordered_map<std::string, FarmlandTile*> farmlands; // ʹ��string����������Ϊkey
    std::vector<Vec2> farmablePositions;

    // ��Vec2ת��Ϊstring key
    std::string positionToString(const Vec2& pos) const {
        return std::to_string(int(pos.x)) + "," + std::to_string(int(pos.y));
    }
};