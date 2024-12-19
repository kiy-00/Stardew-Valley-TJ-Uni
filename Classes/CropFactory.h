#pragma once
#ifndef __CROP_FACTORY_H__
#define __CROP_FACTORY_H__

#include "cocos2d.h"
#include "Enums.h"
#include "Crop.h"
#include "CornCrop.h"
#include <unordered_map>
#include <memory>
class CropFactory {
public:
	// ����ģʽ
	static CropFactory* getInstance();

	// ��������Ĺ�������
	static Crop* createCrop(CropType type);

private:
	
	CropFactory() {}
	static CropFactory* s_instance;
	static const std::unordered_map<CropType, std::function<Crop* ()>> cropCreators;
};

#endif // __CROP_FACTORY_H__