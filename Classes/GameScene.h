// GameScene.h
#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Farmland.h"
#include "ToolManager.h"
#include "SeasonSystem.h"
#include <vector>

class GameScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	~GameScene();

private:
	std::vector<Farmland*> m_farmlands;  // ũ������

	ITool* m_currentTool;                // ��ǰ�ֳֵĹ���
	CropType m_currentCropType;			//��ǰ�ֳֵ�����
	bool m_isHarvestMode;				//�Ƿ����ջ�״̬

	WeatherSystem* weatherSystem;//����ʱ���ϵͳ
	SeasonSystem* seasonSystem;//�����ڵ�ϵͳ

	TMXTiledMap* tmxMap;

	void initFarmlands();    // ��ʼ��ũ������
	void initToolButtons();  // ��ʼ�����߰�ť
	void initCropButtons();	//��ʼ�����Ӱ�ť
	bool initTileMap();

	// �����¼�����
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	// ����ѡ����
	void onToolButtonClicked(Ref* sender, const std::string& toolType);
	void onCropButtonClicked(Ref* sender, CropType cropType);

	// ��ȡ���λ�õ�ũ��
	Farmland* getFarmlandAtPosition(const cocos2d::Vec2& position);

	//��ʱ����
	void update(float dt)override;
};

#endif#pragma once
