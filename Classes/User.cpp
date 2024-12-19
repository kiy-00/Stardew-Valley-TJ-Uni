#include "User.h"

USING_NS_CC;

User* User::create(const std::string& name, const std::string& gender, int health, int energy,int money) {
    User* pRet = new(std::nothrow) User();
    if (pRet && pRet->init(name, gender, health, energy, money)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool User::init(const std::string& name, const std::string& gender, int health, int energy,int money) {
    // Initialize user properties
    m_name = name;
    m_gender = gender;
    m_health = health;
    m_energy = energy;
    m_money = money;
    m_direction = 0; 
    m_isMoving = false;

    heldItemSprite = cocos2d::Sprite::create(); // �����µ� Sprite
    this->addChild(heldItemSprite, 1);  // �������ӵ��û��Ľڵ���
    heldItemSprite->setVisible(false);

    inventory = new Inventory();

    // ��ʼ������״̬
    isInventoryOpen = false; // ������ʼʱ�ǹرյ�

    loadAnimationFrames();

    this->setSpriteFrame(m_animationFrames[0].at(0)); // Default frame (down facing)

    m_frameIndex = 0;  // ��ʼ��֡����
    m_updateCounter = 0;
    schedule(CC_SCHEDULE_SELECTOR(User::updateAnimation), 0.1f);

    return true;
}

void User::updateAnimation(float dt) {
    m_updateCounter++;
    if (m_updateCounter % 5 == 0) {
        if (m_isMoving) {
            m_frameIndex = (m_frameIndex + 1) % 4;  // ����ÿ��������4֡
            heldItemSprite->setVisible(false);
            // ���ݷ�����¾���֡
            this->setSpriteFrame(m_animationFrames[m_direction].at(m_frameIndex));
        }
        else {
            this->setSpriteFrame(m_animationFrames[m_direction].at(0));//վ��
            if (selectedSlot.first != -1 && selectedSlot.second != -1) {
                auto items = inventory->getItems(selectedSlot.first, selectedSlot.second);
                if (!items.empty() && items[0]->getQuantity() > 0) {
                    heldItemSprite->setVisible(true); // ��ʾ��Ʒ
                }
                else {
                    heldItemSprite->setVisible(false); // ������Ʒ
                    return;
                }
            }
            else {
                heldItemSprite->setVisible(false); // ��ûѡ����Ʒ������
                return;
            }
            switch (m_direction) {
            case 0: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(34, 16)); // �·����λ��
                break;
            case 1: // ��
                heldItemSprite->setVisible(false); // �Ϸ����λ��
                break;
            case 2: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(-1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(12, 15)); // �����λ��
                break;
            case 3: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(28, 15)); // �ҷ����λ��
                break;
            }
        }
    }
}
void User::loadAnimationFrames() {
    const std::string directions[4] = { "down", "up", "left", "right" };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::string frameName = StringUtils::format("character/user/user_01/user_0_%s_%d.png.", directions[i].c_str(), j);
            auto frame = SpriteFrame::create(frameName, Rect(0, 0, 40, 60));
            if (frame) {
                m_animationFrames[i].pushBack(frame);
            }
            else {
                CCLOG("Failed to load frame: %s", frameName.c_str());
            }
        }
    }
}

void User::moveDown() {
    m_direction = 0; // ���÷���Ϊ��
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}
void User::moveLeft() {
    m_direction = 2; // ���÷���Ϊ��
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}
void User::moveRight() {
    m_direction = 3; // ���÷���Ϊ��
    m_isMoving = true;
}
void User::moveUp() {
    m_direction = 1; // ���÷���Ϊ��
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}

void User::toggleInventory() {
    m_isMoving = false;

    if (!inventoryLayer) {
        isInventoryOpen = true;
        inventoryLayer = cocos2d::Layer::create();
        auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180));
        inventoryLayer->addChild(background);

        const int slotsPerRow = 8;
        const int rows = 3;
        const float slotSize = 30.0f;

        float startX = (Director::getInstance()->getVisibleSize().width - (slotsPerRow * slotSize)) / 2;
        float startY = (Director::getInstance()->getVisibleSize().height - (rows * slotSize)) / 2;

        background->setPosition(0, 0);

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < slotsPerRow; ++col) {
                auto slot = cocos2d::Sprite::create("pack/slot.png");
                if (slot) {
                    slot->setPosition(Vec2(startX + col * slotSize + slotSize / 2, startY + row * slotSize + slotSize / 2));
                    inventoryLayer->addChild(slot);
                }
            }
        }

        auto trashSlot = cocos2d::Sprite::create("pack/slot.png"); 
        if (trashSlot) {
            trashSlot->setPosition(Vec2(startX + slotsPerRow * slotSize + slotSize / 2, startY + slotSize / 2)); // �������һ����λ���ұ�
            inventoryLayer->addChild(trashSlot);
        }

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < slotsPerRow; ++col) {
                auto slotItems = inventory->getItems(row, col);
                if (!slotItems.empty()) {
                    Item* item = slotItems[0];
                    auto sprite = item->getSprite();

                    if (sprite) {
                        sprite->setPosition(Vec2(startX + col * slotSize + slotSize / 2, startY + row * slotSize + slotSize / 2));
                        inventoryLayer->addChild(sprite);

                        // ��ȡ��Ʒ����
                        int quantity = item->getQuantity();
                        auto quantityLabel = cocos2d::Label::createWithSystemFont(std::to_string(quantity), "Arial", 12);
                        quantityLabel->setPosition(Vec2(startX + col * slotSize + slotSize - 10, startY + row * slotSize + 10));
                        inventoryLayer->addChild(quantityLabel);
                    }
                }
            }
        }

        Director::getInstance()->getRunningScene()->addChild(inventoryLayer, 10, "InventoryLayer");
    }
    else {
        isInventoryOpen = false;
        cocos2d::Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene->getChildByName("InventoryLayer")) {
            currentScene->removeChildByName("InventoryLayer");
        }
        if (currentScene->getChildByName("InventoryBarLayer")) {
            currentScene->removeChildByName("InventoryBarLayer");
        }
        createInventoryBar();
        inventoryLayer = nullptr;
    }
}
void User::onSlotClicked(int row, int col) {
    if (inventoryLayer) {
        // ����Ƿ�������Ͱ��λ 
        if (row == 0 && col == 8) {
            // ����Ͱ��λ��ɾ��ѡ�е���Ʒ
            Item* selectedItem = getSelectedItem();
            if (selectedItem) {
                // �������еļ�����������
                reduceSelectedItemQuantity(selectedItem->getQuantity(),false); 
                updateInventoryDisplay(); // ���±�����ʾ
            }
            return; // ���أ�������������λ
        }

        if (inventory->getItems(row, col).empty()) {
            // �����ǰ��λΪ��
            if (selectedSlot.first != -1 && selectedSlot.second != -1) {
                if (inventory->moveItems(row, col)) {
                    updateInventoryDisplay(); // ������ʾ
                }
            }
        }
        else {
            // ѡ�е�ǰ��λ
            inventory->selectSlot(row, col);
            selectedSlot = { row, col };
            CCLOG("ѡ�в�λ: ��: %d, ��: %d", selectedSlot.first, selectedSlot.second);
            Item* selectedItem = getSelectedItem();
            updateInventoryDisplay();
        }
    }
}
void User::updateInventoryDisplay() {
    // �����ǰ��ʾ
    inventoryLayer->removeAllChildren();

    // ���»��Ʊ���
    const int slotsPerRow = 8; // ÿ�� 8 ������
    const int rows = 3; // 3 ��
    const float slotSize = 30.0f; // ÿ�����ӵĴ�С
    float startX = (Director::getInstance()->getVisibleSize().width - (slotsPerRow * slotSize)) / 2;
    float startY = (Director::getInstance()->getVisibleSize().height - (rows * slotSize)) / 2;

    // ���Ʊ���
    auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180));
    background->setPosition(0, 0);  // ������������������
    inventoryLayer->addChild(background);

    // ������λ
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < slotsPerRow; ++col) {
            auto slot = cocos2d::Sprite::create("pack/slot.png");
            if (slot) {
                // ���ø��ӵľ���λ��
                slot->setPosition(Vec2(startX + col * slotSize + slotSize / 2,
                    startY + row * slotSize + slotSize / 2));
                inventoryLayer->addChild(slot);
            }
        }
    }

    auto trashSlot = cocos2d::Sprite::create("pack/slot.png");
    if (trashSlot) {
        trashSlot->setPosition(Vec2(startX + slotsPerRow * slotSize + slotSize / 2, startY + slotSize / 2)); // �������һ����λ���ұ�
        inventoryLayer->addChild(trashSlot);
    }

    // ���»�����Ʒ
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < slotsPerRow; ++col) {
            auto slotItems = inventory->getItems(row, col);
            if (!slotItems.empty()) {
                Item* item = slotItems[0]; // ��ȡ��һ����Ʒ
                auto sprite = item->getSprite();
                if (sprite) {
                    sprite->setPosition(Vec2(startX + col * slotSize + slotSize / 2,
                        startY + row * slotSize + slotSize / 2));
                    inventoryLayer->addChild(sprite);

                    // ��ȡ����ʾ��Ʒ����
                    int quantity = item->getQuantity(); // ʹ����Ʒ���������ǲ�λ��С
                    auto quantityLabel = cocos2d::Label::createWithSystemFont(std::to_string(quantity), "Arial", 12);
                    quantityLabel->setPosition(Vec2(startX + col * slotSize + slotSize - 10, // ���½�
                        startY + row * slotSize + 10));
                    inventoryLayer->addChild(quantityLabel);
                }
            }
        }
    }
}

void User::createInventoryBar() {
    // ������������
    auto inventoryBarLayer = cocos2d::Layer::create();
    auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0));
    inventoryBarLayer->addChild(background);

    const int slotsPerRow = 8;
    const float slotSize = 30.0f;

    float startX = (Director::getInstance()->getVisibleSize().width - (slotsPerRow * slotSize)) / 2;
    float startY = 0;

    // ���Ӳ�λ
    for (int i = 0; i < slotsPerRow; ++i) {
        auto slot = cocos2d::Sprite::create("pack/slot.png");
        if (slot) {
            slot->setPosition(Vec2(startX + i * slotSize + slotSize / 2, startY + slotSize / 2));
            slot->setTag(i);
            inventoryBarLayer->addChild(slot);
        }
    }

    // ��ȡ������0�е���Ʒ�б������ӵ���Ʒ��
    for (int i = 0; i < slotsPerRow; ++i) {
        auto items = inventory->getItems(0, i);
        if (!items.empty()) {
            Item* item = items[0];
            auto sprite = item->getSprite();

            if (sprite) {
                sprite->setPosition(Vec2(startX + i * slotSize + slotSize / 2, startY + slotSize / 2));
                inventoryBarLayer->addChild(sprite);

                // ��ȡ��Ʒ����
                int quantity = item->getQuantity(); // ��ȡ��Ʒ�����������ǲ�λ������
                auto quantityLabel = cocos2d::Label::createWithSystemFont(std::to_string(quantity), "Arial", 12);
                quantityLabel->setPosition(Vec2(startX + i * slotSize + slotSize - 10, startY + slotSize / 4));
                inventoryBarLayer->addChild(quantityLabel);
            }
        }
    }

    Director::getInstance()->getRunningScene()->addChild(inventoryBarLayer, 10, "InventoryBarLayer");
}
void User::selectItemFromInventory(int index) {

    heldItemSprite->setVisible(false);

    if (index < 0 || index >= 8) {
        return; // ��Ч��������ֱ�ӷ���
    }

    // ��ȡ������
    auto currentBarLayer = Director::getInstance()->getRunningScene()->getChildByName("InventoryBarLayer");
    if (!currentBarLayer) {
        return;
    }

    // �������в�λΪ slot.png
    for (int i = 0; i < 8; ++i) {
        auto slotSprite = dynamic_cast<cocos2d::Sprite*>(currentBarLayer->getChildByTag(i));
        if (slotSprite) {
            slotSprite->setTexture("pack/slot.png"); // �ָ�ΪĬ��״̬
        }
    }
    CCLOG("��ǰѡ�в�λ: ��: %d, ��?: %d", selectedSlot.first, selectedSlot.second);
    CCLOG(" ��?: %d", index);
    // ��ȡ�����ĵ�ǰѡ�в�λ������
    auto selectedSlotSprite = dynamic_cast<cocos2d::Sprite*>(currentBarLayer->getChildByTag(index));
    if (selectedSlotSprite) {
        CCLOG("Changing texture of slot %d to slot_02.png", index);
        selectedSlotSprite->setTexture("pack/slot_02.png"); // ��Ϊѡ��״̬
    }
    else {
        CCLOG("No slot found for index: %d", index);
    }

    // �������ȡ��Ӧ��������Ʒ
    auto items = inventory->getItems(0, index); 

    if (items.empty()||items[0]->getQuantity()==0) {
        // ������������������� heldItemSprite
        CCLOG("not found��items.size() = %lu", items.size());
        heldItemSprite->setVisible(false);
        return; // ȷ��������ִ�к����߼�
    }

    if (!items.empty()) {
        inventory->selectSlot(0, index);
        selectedSlot = { 0, index };
        Item* selectedItem = items[0]; // ��ȡ��Ʒ
        if (selectedItem) {
            // ���� heldItemSprite ����ͼ
            heldItemSprite->setTexture(selectedItem->getImagePath());
            heldItemSprite->setVisible(true);
            switch (m_direction) {
            case 0: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(34, 16)); // �·����λ��
                break;
            case 1: // ��
                heldItemSprite->setVisible(false); // �Ϸ����λ��
                break;
            case 2: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(-1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(12, 15)); // �����λ��
                break;
            case 3: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(28, 15)); // �ҷ����λ��
                break;
            }
        }
    }
}

Item* User::getSelectedItem() {
    if (selectedSlot.first != -1 && selectedSlot.second != -1) {
        auto items = inventory->getItems(selectedSlot.first, selectedSlot.second);
        if (!items.empty()) {
            return items[0]; // ����ѡ�е���Ʒ
        }
    }
    return nullptr;
}

bool User::reduceSelectedItemQuantity(int quantity,bool status) {
 
    if (selectedSlot.first != -1 && selectedSlot.second != -1) {
        // ���� Inventory �ķ������Լ�������
        bool success = inventory->reduceItemQuantity(selectedSlot.first, selectedSlot.second, quantity);

        if (success) {
            if (inventoryLayer) {
                updateInventoryDisplay(); // ���±�����ʾ
            }
            else {
                CCLOG("inventoryLayer ָ��Ϊ�գ����ܸ��±�����ʾ");
            }
            if (status) {
                cocos2d::Scene* currentScene = Director::getInstance()->getRunningScene();
                if (currentScene->getChildByName("InventoryBarLayer")) {
                    currentScene->removeChildByName("InventoryBarLayer");
                }
                createInventoryBar(); // ���ú������º���
            }

           
        
        }
        else {
            CCLOG("������Ʒ����ʧ��: ��Ʒ�����û��ѡ����Ʒ��");
        }
        return success; // ���ؼ��ٽ��
    }
    return false; // ��δѡ����Ʒ��λ�򷵻�ʧ��
}

cocos2d::Sprite* User::getHeldItemSprite() const {
    return heldItemSprite; // ���� heldItemSprite
}

void User::toggleSlotImage() {
    if (slotSprite == nullptr) {
        // ��������ʾ��λͼ��
        slotSprite = cocos2d::Sprite::create("pack/slot_03.png");
        if (slotSprite) {
            slotSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
            Director::getInstance()->getRunningScene()->addChild(slotSprite, 10);
            isSlotImageOpen = true;
        }
    }
    else {
        // �����λͼ���Ѿ����ڣ����Ƴ���
        isSlotImageOpen = false;
        slotSprite->removeFromParent();
        slotSprite = nullptr; // ���ָ��
    }
}