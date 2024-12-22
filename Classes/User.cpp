#include "User.h"
#include "Item.h"
#include "cocos2d.h"
#include "RenderConstants.h"

USING_NS_CC;

////////////////////////////////////////////////////////////
// ���� User ����Ĺ�������
// @param name   �û�����
// @param gender �Ա�
// @param health ����ֵ
// @param energy ����ֵ
// @param money  ��Ǯ����
// @return �����õ� User ָ��
////////////////////////////////////////////////////////////
User* User::create(const std::string& name, const std::string& gender, int health, int energy, int money) {
    User* pRet = new(std::nothrow) User();
    if (pRet && pRet->init(name, gender, health, energy, money)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

////////////////////////////////////////////////////////////
// ��ʼ�� User ����
// �趨�������ԣ����ض���֡����ʼ�� heldItemSprite, inventory �ȡ�
// @param name   �û�����
// @param gender �Ա�
// @param health ����ֵ
// @param energy ����ֵ
// @param money  ��Ǯ����
// @return ��ʼ���Ƿ�ɹ�
////////////////////////////////////////////////////////////
bool User::init(const std::string& name, const std::string& gender, int health, int energy, int money) {
    m_name = name;
    m_gender = gender;
    m_health = health;
    m_energy = energy;
    m_money = money;
    m_direction = 0;
    m_isMoving = false;

    // ��������� heldItemSprite
    heldItemSprite = cocos2d::Sprite::create();
    this->addChild(heldItemSprite, 1);
    heldItemSprite->setVisible(false);

    // ��ʼ������
    inventory = new Inventory();
    isInventoryOpen = false;

    // ���ؽ�ɫ����֡
    loadAnimationFrames();

    // ����Ĭ��վ������֡
    this->setSpriteFrame(m_animationFrames[0].at(0));
    m_frameIndex = 0;
    m_updateCounter = 0;
    // ʹ�� schedule ���ڵ��� updateAnimation �����¶���
    schedule(CC_SCHEDULE_SELECTOR(User::updateAnimation), 0.1f);

    // �������߶�������
    toolActionSprite = cocos2d::Sprite::create();
    this->addChild(toolActionSprite, 2);
    toolActionSprite->setVisible(false);

    return true;
}

////////////////////////////////////////////////////////////
// ���¶����߼���ÿ�� 0.1 �뱻����һ��
// ������ڲ��Ź��߶������������£���������ƶ��뷽����½�ɫ����֡����Ʒ��ʾ��
////////////////////////////////////////////////////////////
void User::updateAnimation(float dt) {
    if (m_isPerformingToolAction) {
        // ���߶��������У�������ͨ��������
        return;
    }

    m_updateCounter++;
    // ÿ 4 �ε��ò��л�һ֡�����Ͷ����л�Ƶ��
    if (m_updateCounter % 4 == 0) {
        if (m_isMoving) {
            // �ƶ��У�ѭ�������ƶ�����
            m_frameIndex = (m_frameIndex + 1) % 4;
            heldItemSprite->setVisible(false);
            this->setSpriteFrame(m_animationFrames[m_direction].at(m_frameIndex));
        }
        else {
            // ��ֹʱʹ��վ��֡
            this->setSpriteFrame(m_animationFrames[m_direction].at(0));
            // ���ѡ�в�λ�Ƿ�����Ʒ
            if (selectedSlot.first != -1 && selectedSlot.second != -1) {
                auto items = inventory->getItems(selectedSlot.first, selectedSlot.second);
                if (!items.empty() && items[0]->getQuantity() > 0) {
                    heldItemSprite->setVisible(true);
                }
                else {
                    heldItemSprite->setVisible(false);
                    return;
                }
            }
            else {
                heldItemSprite->setVisible(false);
                return;
            }
            // ���ݷ���ڷ���Ʒλ���뷽��
            switch (m_direction) {
            case 0: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(Vec2(34, 16));
                break;
            case 1: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(Vec2(36, 20));
                break;
            case 2: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(-1.0f);
                heldItemSprite->setPosition(Vec2(12, 15));
                break;
            case 3: // ��
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(Vec2(28, 15));
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////
// ���ؽ�ɫ���߶���֡
// �Ӷ�Ӧ·��������������4������ÿ����4֡�����߶�����
////////////////////////////////////////////////////////////
void User::loadAnimationFrames() {
    const std::string directions[4] = { "down", "up", "left", "right" };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::string frameName = StringUtils::format("character/user/user_01/user_0_%s_%d.png", directions[i].c_str(), j);
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

////////////////////////////////////////////////////////////
// ��ȡ���߶���֡
// ���ݹ������ͷ�����ض�Ӧ��plist�����л�ȡ��֡������
// @param toolName �������ƣ�����չ����
// @param direction ����(0��,1��,2��,3��)
// @return ������֡���߶����� Vector<SpriteFrame*>
////////////////////////////////////////////////////////////
Vector<SpriteFrame*> User::getToolAnimationFrames(const std::string& toolName, int direction) {
    Vector<SpriteFrame*> frames;
    std::string directionName;
    switch (direction) {
    case 0: directionName = "down"; break;
    case 1: directionName = "up"; break;
    case 2: directionName = "left"; break;
    case 3: directionName = "right"; break;
    default: directionName = "down"; break;
    }

    // ���ض�Ӧ�����plist�ļ�
    std::string plistFile = StringUtils::format("tool/plist/%s_%s.plist", toolName.c_str(), directionName.c_str());
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile);
    CCLOG("Loaded plist file: %s", plistFile.c_str());

    // ���Լ�����֡����
    for (int i = 0; i < 2; ++i) {
        std::string frameName = StringUtils::format("%s_%s_%d.png", toolName.c_str(), directionName.c_str(), i);
        CCLOG("Attempting to load frame: %s", frameName.c_str());
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
            CCLOG("Successfully loaded frame: %s", frameName.c_str());
        }
        else {
            CCLOG("Failed to load frame: %s", frameName.c_str());
        }
    }

    return frames;
}

////////////////////////////////////////////////////////////
// ���� moveXXX �����������ý�ɫ�ƶ����򲢿�ʼ�ƶ�����
// ͬʱ���� heldItemSprite ��ֹ�ƶ���������Ʒ��ʾ����Ȼ
////////////////////////////////////////////////////////////
void User::moveDown() {
    m_direction = 0;
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}
void User::moveLeft() {
    m_direction = 2;
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}
void User::moveRight() {
    m_direction = 3;
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}
void User::moveUp() {
    m_direction = 1;
    m_isMoving = true;
    heldItemSprite->setVisible(false);
}

////////////////////////////////////////////////////////////
// toggleInventory �л�������ʾ״̬
// �����ǰ����δ��ʾ�򴴽�����UI�㲢��ʾ��
// �������ʾ���Ƴ�����UI�㡣
////////////////////////////////////////////////////////////
void User::toggleInventory() {
    m_isMoving = false;

    if (!inventoryLayer) {
        isInventoryOpen = true;
        inventoryLayer = cocos2d::Layer::create();

        const int slotsPerRow = 8;
        const int rows = 3;
        const float slotSize = 30.0f;

        // ���ñ�������λ����������
        inventoryLayer->setPosition(this->getPosition() + Vec2(-125, 100));

        // ������͸������
        auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180));
        background->setContentSize(cocos2d::Size(slotsPerRow * slotSize + slotSize, rows * slotSize + slotSize));
        background->setPosition(-slotSize / 2, -slotSize / 2);
        inventoryLayer->addChild(background);

        // ����������λ
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < slotsPerRow; ++col) {
                auto slot = cocos2d::Sprite::create("pack/slot.png");
                if (slot) {
                    slot->setPosition(Vec2(col * slotSize + slotSize / 2, row * slotSize + slotSize / 2));
                    inventoryLayer->addChild(slot);
                }
            }
        }

        // ���������ۣ����Ҳࣩ
        auto trashSlot = cocos2d::Sprite::create("pack/garbage.png");
        if (trashSlot) {
            trashSlot->setPosition(Vec2(slotsPerRow * slotSize + slotSize / 2, slotSize / 2));
            inventoryLayer->addChild(trashSlot);
        }

        // ��ʾ��Ʒ
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < slotsPerRow; ++col) {
                auto slotItems = inventory->getItems(row, col);
                if (!slotItems.empty()) {
                    Item* item = slotItems[0];
                    auto sprite = item->getSprite();
                    if (sprite) {
                        sprite->setPosition(Vec2(col * slotSize + slotSize / 2, row * slotSize + slotSize / 2));
                        inventoryLayer->addChild(sprite);

                        int quantity = item->getQuantity();
                        auto quantityLabel = cocos2d::Label::createWithSystemFont(std::to_string(quantity), "Arial", 12);
                        quantityLabel->setPosition(Vec2(col * slotSize + slotSize - 10, row * slotSize + 10));
                        inventoryLayer->addChild(quantityLabel);
                    }
                }
            }
        }
        Director::getInstance()->getRunningScene()->addChild(inventoryLayer, FOUR, "InventoryLayer");
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

////////////////////////////////////////////////////////////
// onSlotClicked ��Ӧ������λ����¼�
// ���ݵ���Ĳ�λѡ����Ʒ���ƶ���Ʒ��
////////////////////////////////////////////////////////////
void User::onSlotClicked(int row, int col) {
    if (!inventoryLayer) return;

    // ����Ƿ�������Ͱ��λ 
    if (row == 0 && col == 8) {
        // �����ǰ��ѡ����Ʒ������ȫ��ɾ��
        if (selectedSlot.first != -1 && selectedSlot.second != -1) {
            Item* selectedItem = getSelectedItem();
            if (selectedItem) {
                reduceSelectedItemQuantity(selectedItem->getQuantity(), false);
                // ����ѡ�в�λ
                selectedSlot = { -1, -1 };
                heldItemSprite->setVisible(false);
                updateInventoryDisplay();
            }
        }
        return;
    }

    // ��ȡ��ǰ��λ����Ʒ�б�
    auto slotItems = inventory->getItems(row, col);
    bool slotEmpty = slotItems.empty() || slotItems[0]->getQuantity() == 0;

    // ���1����ǰδѡ���κ���Ʒ
    if (selectedSlot.first == -1 && selectedSlot.second == -1) {
        if (!slotEmpty) {
            // ѡ�е�ǰ��λ����Ʒ
            inventory->selectSlot(row, col);
            selectedSlot = { row, col };
            updateHeldItemSprite();
            updateInventoryDisplay();
        }
        return;
    }
    // ���2����ǰ�Ѿ�ѡ��һ����Ʒ
    if (slotEmpty) {
        // ��ѡ����Ʒ�ƶ����ÿղ�λ
        if (inventory->moveItems(row, col)) {
            selectedSlot = { -1, -1 };
            heldItemSprite->setVisible(false);
            updateInventoryDisplay();
        }
        return;
    }
    else {
        // ѡ���µ���Ʒ
        inventory->selectSlot(row, col);
        selectedSlot = { row, col };
        updateHeldItemSprite();
        updateInventoryDisplay();
    }
}

////////////////////////////////////////////////////////////
// updateHeldItemSprite ���³�����Ʒ��ʾ
// ���ݵ�ǰ�Ƿ�ѡ����Ʒ������ʾ��
////////////////////////////////////////////////////////////
void User::updateHeldItemSprite() {
    Item* selectedItem = getSelectedItem();
    if (selectedItem) {
        heldItemSprite->setTexture(selectedItem->getImagePath());
        heldItemSprite->setVisible(true);

        // ���ݵ�ǰ����������Ʒ��λ�úͳ���
        switch (m_direction) {
        case 0: // ��
            heldItemSprite->setScaleX(1.0f);
            heldItemSprite->setPosition(Vec2(34, 16));
            break;
        case 1: // ��
            heldItemSprite->setScaleX(1.0f);
            heldItemSprite->setPosition(Vec2(36, 20));
            break;
        case 2: // ��
            heldItemSprite->setScaleX(-1.0f);
            heldItemSprite->setPosition(Vec2(12, 15));
            break;
        case 3: // ��
            heldItemSprite->setScaleX(1.0f);
            heldItemSprite->setPosition(Vec2(28, 15));
            break;
        }
    }
    else {
        heldItemSprite->setVisible(false);
    }
}

////////////////////////////////////////////////////////////
// updateInventoryDisplay ���±���������ʾ
// ���ݵ�ǰ������Ʒ��Ϣ���»��Ʊ���UI��
////////////////////////////////////////////////////////////
void User::updateInventoryDisplay() {
    if (!inventoryLayer) return;

    inventoryLayer->removeAllChildren();  // �����ǰ��ʾ����������

    const int slotsPerRow = 8;
    const int rows = 3;
    const float slotSize = 30.0f;

    // ���Ʊ���
    auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180));
    background->setContentSize(cocos2d::Size(slotsPerRow * slotSize + slotSize, rows * slotSize + slotSize));
    background->setPosition(-slotSize / 2, -slotSize / 2);
    inventoryLayer->addChild(background);

    // ���Ʊ�����λ
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < slotsPerRow; ++col) {
            auto slot = cocos2d::Sprite::create("pack/slot.png");
            slot->setPosition(Vec2(col * slotSize + slotSize / 2, row * slotSize + slotSize / 2));
            inventoryLayer->addChild(slot);
        }
    }

    // ��������Ͱ��λ
    auto trashSlot = cocos2d::Sprite::create("pack/garbage.png");
    trashSlot->setPosition(Vec2(slotsPerRow * slotSize + slotSize / 2, slotSize / 2));
    inventoryLayer->addChild(trashSlot);

    // ���»�����Ʒ
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < slotsPerRow; ++col) {
            auto slotItems = inventory->getItems(row, col);
            if (!slotItems.empty()) {
                Item* item = slotItems[0];
                auto sprite = item->getSprite();
                sprite->setPosition(Vec2(col * slotSize + slotSize / 2, row * slotSize + slotSize / 2));
                inventoryLayer->addChild(sprite);

                int quantity = item->getQuantity();
                auto quantityLabel = cocos2d::Label::createWithSystemFont(std::to_string(quantity), "Arial", 12);
                quantityLabel->setPosition(Vec2(col * slotSize + slotSize - 10, row * slotSize + 10));
                inventoryLayer->addChild(quantityLabel);
            }
        }
    }
}

////////////////////////////////////////////////////////////
// createInventoryBar ����������Ʒ��
// ��ʾ������0�е���Ʒ����Ļ�·��ĺ����С�
////////////////////////////////////////////////////////////
void User::createInventoryBar() {
    // ������������
    auto inventoryBarLayer = cocos2d::Layer::create();
    auto background = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0));
    inventoryBarLayer->addChild(background);

    const int slotsPerRow = 8;
    const float slotSize = 30.0f;

    float startX = (Director::getInstance()->getVisibleSize().width - (slotsPerRow * slotSize)) / 2;
    float startY = 0;

    // ��Ӳ�λ
    for (int i = 0; i < slotsPerRow; ++i) {
        auto slot = cocos2d::Sprite::create("pack/slot.png");
        if (slot) {
            slot->setPosition(Vec2(startX + i * slotSize + slotSize / 2, startY + slotSize / 2));
            slot->setTag(i);
            inventoryBarLayer->addChild(slot);
        }
    }
    // ��ȡ������0�е���Ʒ�б���ӵ���Ʒ��
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
    Director::getInstance()->getRunningScene()->addChild(inventoryBarLayer, FOUR, "InventoryBarLayer");
}

////////////////////////////////////////////////////////////
// selectItemFromInventory �Ӻ���ѡ����Ʒ��λ
// ��ʾѡ����Ʒ��ͼ�꣨heldItemSprite��������״̬��
////////////////////////////////////////////////////////////
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

    if (items.empty() || items[0]->getQuantity() == 0) {
        // ������������������ heldItemSprite
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
                heldItemSprite->setVisible(true);
                heldItemSprite->setScaleX(1.0f);
                heldItemSprite->setPosition(cocos2d::Vec2(36, 20)); // �Ϸ����λ��
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

////////////////////////////////////////////////////////////
// getSelectedItem ��ȡ��ǰѡ�е���Ʒ
// @return ��ǰѡ�в�λ����Ʒָ�룬���û��ѡ�з���nullptr
////////////////////////////////////////////////////////////
Item* User::getSelectedItem() {
    if (selectedSlot.first != -1 && selectedSlot.second != -1) {
        auto items = inventory->getItems(selectedSlot.first, selectedSlot.second);
        if (!items.empty()) {
            return items[0];
        }
    }
    return nullptr;
}

////////////////////////////////////////////////////////////
// reduceSelectedItemQuantity ����ѡ����Ʒ����
// @param quantity Ҫ���ٵ�����
// @param status �Ƿ���Ҫ���º���
// @return �Ƿ�ɹ���������
////////////////////////////////////////////////////////////
bool User::reduceSelectedItemQuantity(int quantity, bool status) {

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

////////////////////////////////////////////////////////////
// getHeldItemSprite ��ȡheldItemSprite����ָ��
// ������ʾ��ǰ�ֳ���Ʒ��ͼ�ꡣ
////////////////////////////////////////////////////////////
cocos2d::Sprite* User::getHeldItemSprite() const {
    return heldItemSprite; // ���� heldItemSprite
}

////////////////////////////////////////////////////////////
// toggleSlotImage ��ʾ�����ز�λͼ��
// ��� slotSprite Ϊ���򴴽���ʾ�������Ƴ�
////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
// @brief  ��鲢ʰȡ��Ҹ�����Χ�ڵ���Ʒ
// @param  items        ���������а��� Item ����� Sprite �б�
// @param  pickupRadius ʰȡ��Χ�뾶��������Ϊ��λ��
// 
// @details
// ���������� Sprite �б����ÿ����Ʒ�����֮��ľ��룬
// �����Ʒ��ʰȡ��Χ�ڣ������Ӧ�� Item ʵ����ӵ���ҵı�����
// ���ӳ������Ƴ�����Ʒ�� Sprite��
// 
// @note
// - ÿ����Ʒ�� Sprite �������Ч�� Item ������Ϊ UserData��
// - ��Ʒʰȡ��Item ʵ���ᱻ��ӵ������У�Sprite �ӳ������Ƴ���
////////////////////////////////////////////////////////////
void User::pickupNearbyItems(const std::vector<cocos2d::Sprite*>& items, float pickupRadius) {
	Vec2 playerPos = this->getPosition();

	for (auto sprite : items) {
		if (!sprite) continue;

		// �����Ʒ�Ƿ���ʰȡ��Χ��
		Vec2 itemPos = sprite->getPosition();
		float distance = playerPos.distance(itemPos);
		if (distance <= pickupRadius) {
			// ��ȡ Sprite �󶨵� Item ����
			void* userData = sprite->getUserData();
			if (!userData) continue;

			// ��ʽת��Ϊ Item*
			Item* item = static_cast<Item*>(userData); 
			if (item) { 
				// ����Ʒ���뱳��
				inventory->addItem(item);

				// �ӳ������Ƴ���Ʒ
				sprite->removeFromParent();
			}
		}
	}
}


////////////////////////////////////////////////////////////
// performAction ͨ�ö�����ں���
// ����Ʒ����ִ�в�ͬ�߼�
////////////////////////////////////////////////////////////
void User::performAction(FarmMapManager* farmMapManager) {
    Item* currentItem = getSelectedItem();
    if (!currentItem) {
        CCLOG("No item selected.");
        return;
    }

    std::string itemType = currentItem->getItemType();
    if (itemType == "tool") {
        performToolAction();
    }
    else if (itemType == "seed") {
        plantSeed();
    }
    else if (itemType == "rod") {
        if (farmMapManager->isFishingSpot(getPosition())) {
            startFishing();
        }
    }
    else {
        CCLOG("No specific action defined for item type: %s", itemType.c_str());
    }
}

////////////////////////////////////////////////////////////
// performToolAction ִ�й��߶�������
// ����ɫ�ֳֹ��߲����������ʱ���Ź��߶�����
// �����ڼ�����heldItemSprite������������ʾ��
////////////////////////////////////////////////////////////
void User::performToolAction() {
    Item* currentItem = getSelectedItem();
    if (!currentItem || currentItem->getItemType().find("tool") == std::string::npos) {
        CCLOG("No valid tool selected.");
        return;
    }

    m_isMoving = false;
    m_isPerformingToolAction = true; // ��ʼ���߶���
    if (heldItemSprite) {
        heldItemSprite->setVisible(false); // �����ֳֵ���
    }

    std::string toolImagePath = currentItem->getImagePath();
    std::string toolName;
    size_t lastSlash = toolImagePath.find_last_of('/');
    size_t dotPos = toolImagePath.find_last_of('.');
    if (lastSlash != std::string::npos && dotPos != std::string::npos && dotPos > lastSlash) {
        toolName = toolImagePath.substr(lastSlash + 1, dotPos - (lastSlash + 1));
    }
    else {
        CCLOG("Failed to parse tool name.");
        m_isPerformingToolAction = false;
        return;
    }

    auto frames = getToolAnimationFrames(toolName, m_direction);
    if (frames.empty()) {
        CCLOG("No tool frames found for %s", toolName.c_str());
        m_isPerformingToolAction = false;
        return;
    }

    toolActionSprite->setVisible(true);
    toolActionSprite->setSpriteFrame(frames.front());
    toolActionSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    toolActionSprite->setPosition(Vec2(0, 0));

    if (toolName == "kettle") {
        switch (m_direction) {
        case 0: toolActionSprite->setPosition(Vec2(30, 16)); break; // ��
        case 1: toolActionSprite->setPosition(Vec2(32, 18)); break; // ��
        case 2: toolActionSprite->setPosition(Vec2(8, 20));  break; // ��
        case 3: toolActionSprite->setPosition(Vec2(32, 20)); break; // ��
        }
    }
    else {
        switch (m_direction) {
        case 0: toolActionSprite->setPosition(Vec2(30, 16)); break; // ��
        case 1: toolActionSprite->setPosition(Vec2(30, 24)); break; // ��
        case 2: toolActionSprite->setPosition(Vec2(8, 20));  break; // ��
        case 3: toolActionSprite->setPosition(Vec2(32, 20)); break; // ��
        }
        toolActionSprite->setScale(1.5f);
    }

    Animation* animation = Animation::createWithSpriteFrames(frames, 0.2f);
    Animate* animate = Animate::create(animation);

    auto callback = CallFunc::create([this]() {
        toolActionSprite->setVisible(false);
        toolActionSprite->setScale(1.0f); // ����������ָ�ԭʼ��С
        if (heldItemSprite) {
            heldItemSprite->setVisible(true);
        }
        m_isPerformingToolAction = false;
        CCLOG("Tool action animation completed.");
        });


    toolActionSprite->stopAllActions();
    toolActionSprite->runAction(Sequence::create(animate, callback, nullptr));
    CCLOG("Tool action animation started for tool: %s", toolName.c_str());
}

////////////////////////////////////////////////////////////
// plantSeed ִ����ֲ�߼�
////////////////////////////////////////////////////////////
void User::plantSeed() {
    CCLOG("Planting a seed at current location.");
}


////////////////////////////////////////////////////////////
// startFishing ִ�е����߼�
// ����С��Ϸ + �����ѡ�� + �� G ���뱳��
////////////////////////////////////////////////////////////
void User::startFishing() {
	CCLOG("Starting fishing game in current scene.");

	// ���� FishingLayer
	auto fishingLayer = FishingLayer::create();

	// ����С��Ϸ����ʱ�Ļص�
	fishingLayer->onGameEnd = [weakFishingLayer = fishingLayer, this](bool success, const FishInfo& fish) {
		auto fishingLayer = weakFishingLayer; // �ָ�ǿ����
		if (!fishingLayer) return;            // �� FishingLayer �����٣�������
		if (!success) return;                 // ���С��Ϸʧ�ܣ�����ʾ�κζ���

        // ���ݵ�ǰ���ڻ�����ɸѡ��
        std::vector<FishInfo> allFish = FishDataFactory::createFishData();

        // �����ѡһ����
        int randomIndex = RandomHelper::random_int(0, (int)allFish.size() - 1);
        FishInfo candidateFish = allFish[randomIndex];

		auto currentScene = Director::getInstance()->getRunningScene();
		if (!currentScene) return;

		// ������� Sprite
		auto fishSprite = Sprite::create(candidateFish.spriteName);

		// ���������ʾλ���������Ϸ�
		auto playerPos = this->getPosition();
		fishSprite->setPosition(playerPos + Vec2(20, 10));
		fishSprite->setScale(1.5f);

		// ���� FishItem ���󶨵� Sprite
		FishItem* fishItem = new FishItem(candidateFish.name, "A freshly caught fish.", candidateFish.spriteName);
        fishSprite->setUserData(static_cast<void*>(fishItem));

		// ����ͼ��ӵ�������
		currentScene->addChild(fishSprite, SIX);
		};

	// ���� FishingLayer ��λ����������
	auto playerPosition = this->getPosition();
	fishingLayer->setPosition(playerPosition - Vec2(120, 0));

	// ��С���� FishingLayer ����Ϊ 50%
	fishingLayer->setScale(0.5f);

	// �� FishingLayer ��ӵ���ǰ����
	auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
	currentScene->addChild(fishingLayer, SIX);
}
