#include "CropManager.h"

USING_NS_CC;

CropManager* CropManager::s_instance = nullptr;

CropManager* CropManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new (std::nothrow) CropManager();
    }
    return s_instance;
}

CropManager::CropManager()
{
}

CropManager::~CropManager()
{
    // ����ɸ�����Ҫ����β�������Ƴ�����crop
    // ��һ������Ϸ�˳�ʱ��Ref����������
}

bool CropManager::init(cocos2d::Scene* scene, int zOrder)
{
    this->_cropLayer = cocos2d::Layer::create();

    if (!_cropLayer)
        return false;

    scene->addChild(_cropLayer, zOrder);

    return true;

}

Crop* CropManager::plantCrop(const std::string& cropType, FarmlandTile* tile)
{
    if (!tile) {
        CCLOG("CropManager::plantCrop failed: tile is nullptr");
        return nullptr;
    }

    // ����������Ѿ�������Ͳ����ظ���ֲ��Ҳ��ѡ���� remove ����ֲ��
    auto it = _cropMap.find(tile);
    if (it != _cropMap.end() && it->second != nullptr) {
        CCLOG("CropManager::plantCrop failed: tile already has a Crop.");
        return nullptr;
    }

    // ����Crop����ע�⣬���Crop::createǩ���� (type, tile)
    auto newCrop = Crop::create(cropType, tile);
    if (!newCrop) {
        CCLOG("CropManager::plantCrop failed: Crop::create returned null for type=%s", cropType.c_str());
        return nullptr;
    }

    // ������������ؿ��Ӧ����������λ��
    // ��ΪFarmlandTile��Node������ tile->getPosition() ����������
    auto pos = tile->getPosition();
    pos.x += 16.0f;
    pos.y += 16.0f;
    newCrop->setPosition(pos);

    // �����Node����ӵ�ͼ�㣬������Ⱦ
    if (_cropLayer) {
        _cropLayer->addChild(newCrop);
    }

    // ��¼����ͼ
    _cropMap[tile] = newCrop;

    CCLOG("CropManager::plantCrop success: Planted %s at tile(%p).", cropType.c_str(), tile);

    return newCrop;
}

Crop* CropManager::getCropOnTile(FarmlandTile* tile) const
{
    auto it = _cropMap.find(tile);
    if (it != _cropMap.end()) {
        return it->second;
    }
    return nullptr;
}

bool CropManager::removeCrop(FarmlandTile* tile, bool alsoCleanup)
{
    auto it = _cropMap.find(tile);
    if (it == _cropMap.end() || it->second == nullptr) {
        CCLOG("CropManager::removeCrop: No crop found on tile(%p).", tile);
        return false;
    }

    // ��ȡ��Ӧ������
    Crop* crop = it->second;

    // �����Ҫ�����¼������ʹӳ����Ƴ�
    if (alsoCleanup) {
        crop->removeFromParent();
    }

    // ��map���Ƴ�
    _cropMap.erase(it);

    CCLOG("CropManager::removeCrop: Crop removed from tile(%p).", tile);
    return true;
}
