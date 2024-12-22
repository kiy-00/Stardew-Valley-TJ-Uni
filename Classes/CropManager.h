#ifndef __CROP_MANAGER_H__
#define __CROP_MANAGER_H__

#include "cocos2d.h"
#include "Crop.h"
#include "FarmlandTile.h"

/**
 * CropManager ����
 * 1. ��¼��FarmlandTile -> Crop����ӳ�䡣
 * 2. �ṩ���֡��Ƴ�����Ƚӿڡ�
 * 3. ������Ŀ��ӻ��ڵ㣨Crop������ӵ�ָ��ͼ���С�
 *
 * ע�⣺��ʾ�������𡰶�ʱ���¡����������Ϊ����� Crop.cpp �
 *       �Ѿ���ÿ�� Crop ������ schedule() �������Լ�������Ҫͳһ������£�
 *       ���ڴ˹������б����������ﲢ���� update()��
 */
class CropManager
{
public:
    /**
     * ��ȡ����
     */
    static CropManager* getInstance();

    /**
     * ��ʼ�����������ɴ���һ��ͼ�㣬���ڽ� Crop �Ľڵ���ӵ���ͼ����Ⱦ��
     * �������Ŀ��Ҫ��������Ⱦ����ũ����ͬ��ͼ�㣬Ҳ����ֱ�Ӵ��� farmlandLayer��
     */
    bool init(cocos2d::Scene* scene, int zOrder);

    /**
     * ��ָ���ĵؿ��ϲ���һ��ָ�����͵�����
     * @param cropType  ��������(��Ӧ��Crop���õ���type������"Berry"��)
     * @param tile      ����Ҫ��ֲ�ĵؿ�(�Ѿ�����/����ֲ)
     * @return ���ɹ�����������ָ��Crop��ָ�룻��ʧ��(���Tile��������)������nullptr
     */
    Crop* plantCrop(const std::string& cropType, FarmlandTile* tile);

    /**
     * ���ݵؿ��ȡ��ǰ������ؿ���������򷵻�nullptr��
     */
    Crop* getCropOnTile(FarmlandTile* tile) const;

    /**
     * �Ƴ��ؿ��϶�Ӧ��������еĻ���
     * @param tile  ��Ӧ�ĵؿ�
     * @param alsoCleanup �Ƿ����Ƴ�ʱ����Crop�� removeEventListeners() �� removeFromParent()
     * @return true���ɹ��Ƴ���false��û���ҵ���Ӧ������
     */
    bool removeCrop(FarmlandTile* tile, bool alsoCleanup = true);

private:
    // ���� & ����
    CropManager();
    ~CropManager();

    // ���ÿ�������͸�ֵ
    CropManager(const CropManager&) = delete;
    CropManager& operator=(const CropManager&) = delete;

private:
    // �洢���ؿ� -> �����ӳ��
    std::unordered_map<FarmlandTile*, Crop*> _cropMap;

    // ������Ⱦ�����ͼ�㣨����FarmlandManager�� farmlandLayer��ͬ���򵥶��½���
    cocos2d::Layer* _cropLayer = nullptr;

    // ������̬ʵ��
    static CropManager* s_instance;
};

#endif // __CROP_MANAGER_H__
#pragma once
