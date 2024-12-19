// User.h
#ifndef __USER_H__
#define __USER_H__

#include "cocos2d.h"
#include "Inventory.h"

class User : public cocos2d::Sprite {
public:
    static User* create(const std::string& name, const std::string& gender, int health, int energy, int money);
    bool init(const std::string& name, const std::string& gender, int health, int energy, int money);

    // �������
    void loadAnimationFrames();
    void updateAnimation(float dt);

    // �ƶ����
    void moveDown();
    void moveUp();
    void moveLeft();
    void moveRight();
    void stopMoving() { m_isMoving = false; }

    // ����ϵͳ
    void toggleInventory();
    void createInventoryBar();
    void updateInventoryDisplay();
    void onSlotClicked(int row, int col);
    void selectItemFromInventory(int index);

    // ��Ʒϵͳ
    Item* getSelectedItem();
    bool reduceSelectedItemQuantity(int quantity, bool status);
    cocos2d::Sprite* getHeldItemSprite() const;
    void toggleSlotImage();

    // ������͸���ȿ���
    void updateVisibility(bool isPenetrable) {
        this->setOpacity(isPenetrable ? 128 : 255);
    }

    // ����������ϵͳ��ʼ��
    void initPhysics() {
        cocos2d::Size playerSize = this->getContentSize();
        auto physicsBody = cocos2d::PhysicsBody::createBox(
            playerSize,
            cocos2d::PhysicsMaterial(1.0f, 0.0f, 1.0f)
        );

        physicsBody->setDynamic(true);
        physicsBody->setRotationEnable(false);
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(0xFFFFFFFF);
        physicsBody->setCollisionBitmask(0xFFFFFFFF);
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        physicsBody->setEnabled(true);
        physicsBody->setTag(1);

        this->setPhysicsBody(physicsBody);
    }

private:
    // ��������
    std::string m_name;
    std::string m_gender;
    int m_health;
    int m_energy;
    int m_money;

    // �������
    int m_direction;
    bool m_isMoving;
    int m_frameIndex;
    int m_updateCounter;
    cocos2d::Vector<cocos2d::SpriteFrame*> m_animationFrames[4];

    // �������
    Inventory* inventory;
    cocos2d::Layer* inventoryLayer;
    bool isInventoryOpen;
    std::pair<int, int> selectedSlot;

    // ��Ʒ���
    cocos2d::Sprite* heldItemSprite;
    cocos2d::Sprite* slotSprite;
    bool isSlotImageOpen;
};

#endif
