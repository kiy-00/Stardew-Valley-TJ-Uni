#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"
#include "AnimalConfig.h"
#include <functional>
#include <unordered_map>
#include <unordered_set>


class Animal : public cocos2d::Sprite
{
public:
	// ���嶯����ƶ������������ң�
	enum class Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	static Animal* createWithType(const std::string& typeName);    // ��̬���������������������
	bool initWithConfig(const AnimalConfig& config);               // ��ʼ�����������ö������ã�
	void touchAnimal();                                            // ���ﱻ����ʱ����
	void update(float dt) override;                                // ÿ֡����ʱ����
	void startRandomMove();                                        // ��ʼ����ƶ�
	std::string getAnimalType() const { return _config.typeName; } // ��ȡ��������

	std::function<void(const std::string&, const cocos2d::Vec2&)> produceCallback;  // �ص��������ͣ�����������Ʒ

protected:
	AnimalConfig _config;         // ���������
	int _favor;                   // ����ĺøж�
	bool _canProduce;             // ��ʾ�����Ƿ���Բ�����Ʒ
	cocos2d::Action* _currentAction; // ��ǰִ�еĶ���

	static std::unordered_set<std::string> _loadedPlists; // �Ѽ��ص�plist�ļ�

	void tryProduce();                      // ������Ʒ
	void playIdleAnimation();               // ���Ŷ��ﾲֹ����
	void playWalkAnimation(Direction dir);  // ���Ŷ������߶���
	void onMoveFinished();                  // ��������
	void doRandomMove();                    // ִ������ƶ�
	Direction getDirectionForMovement(const cocos2d::Vec2& start, const cocos2d::Vec2& end); // �����ƶ�����

	void loadAnimationFrames(const std::string& state, Direction direction); // ���ء�����֡��
	cocos2d::Animate* createAnimate(const std::string& framePrefix, int frameCount, float delayPerUnit = 0.2f); // ��������

	static const std::unordered_map<std::string, AnimalConfig>& getAnimalConfigs();  // ���ù���
};

#endif // __ANIMAL_H__
