#ifndef __ANIMAL_CONFIG_H__
#define __ANIMAL_CONFIG_H__

#include <string>
#include "cocos2d.h"


struct AnimalConfig 
{
	std::string typeName;             // �����������ƣ��� "chicken", "sheep"
	std::string produceItem;          // ������Ʒ���ƣ��� "egg.png", "wool.png"
	std::string idleAnimationPrefix;  // ���ж���ǰ׺���� "chicken_idle_", "sheep_idle_"
	std::string walkAnimationPrefix;  // ���߶���ǰ׺���� "chicken_walk_", "sheep_walk_"
	int produceFavorThreshold;        // ������Ʒ����øж���ֵ
	int favorIncrement;               // ÿ�δ������ӵĺøж�
};

#endif // __ANIMAL_CONFIG_H__

