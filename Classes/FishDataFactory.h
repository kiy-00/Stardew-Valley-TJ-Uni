#ifndef __FISH_DATA_FACTORY_H__
#define __FISH_DATA_FACTORY_H__

#include <vector>
#include "FishInfo.h"

/**
 * @class FishDataFactory
 * @brief �������ݹ�����
 * ���𴴽����㳡���е������ݣ��������ɶ�����������Ϣ��
 */
class FishDataFactory {
public:
    static std::vector<FishInfo> createFishData();  // ����һ��������������Ϣ������
};

#endif


