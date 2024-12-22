#ifndef __FISH_INFO_H__
#define __FISH_INFO_H__

#include <string>

/**
 * @struct FishInfo
 * @brief �洢�����Ϣ���������ڡ����������ơ�ͼ��ȡ�
 */
struct FishInfo {
    std::string season;       // ����: �������ļ����＾������
    std::string weather;      // ����: ���졢����
    std::string name;         // �������
    std::string spriteName;   // ���ͼ��·��
    int gold;                 // ���ֵ
    int hpRecover;            // �ظ�����ֵ
    int energyRecover;        // �ظ�����ֵ
};

#endif // __FISH_INFO_H__

