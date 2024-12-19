#include "Inventory.h"

Inventory::Inventory() {}

Inventory::~Inventory() {
    for (int row = 0; row < ROWS; ++row) {
        for (int slot = 0; slot < SLOTS_PER_ROW; ++slot) {
            for (Item* item : items[row][slot]) {
                delete item; // ȷ���ͷ��ڴ�
            }
        }
    }
}

bool Inventory::addItem(Item* item) {
    for (int row = 0; row < ROWS; ++row) {
        for (int slot = 0; slot < SLOTS_PER_ROW; ++slot) {
            // ����λ�Ƿ��пռ�
            if (items[row][slot].size() < MAX_ITEMS_PER_SLOT) {
                if (items[row][slot].empty() || items[row][slot][0]->getName() == item->getName()) {
                    // �����λ�д�����ͬ���Ƶ���Ʒ������������
                    if (!items[row][slot].empty()) {
                        int newQuantity = items[row][slot][0]->getQuantity() + item->getQuantity();
                        if (newQuantity > MAX_ITEMS_PER_SLOT) {
                            continue;
                        }
                        items[row][slot][0]->setQuantity(newQuantity);
                    }
                    else {
                        items[row][slot].push_back(item); // �����Ʒ
                    }
                    return true; // �ɹ������Ʒ
                }
            }
        }
    }
    return false; // û�п��ò�λ�򲻷�������
}

bool Inventory::reduceItemQuantity(int row, int slot, int quantity) {
    if (row < ROWS && slot < SLOTS_PER_ROW) {
        auto& itemsInSlot = items[row][slot];
        if (!itemsInSlot.empty()) {
            Item* item = itemsInSlot[0];
            int currentQuantity = item->getQuantity();
            if (currentQuantity >= quantity && currentQuantity - quantity >= 0) {
                item->setQuantity(currentQuantity - quantity);
                // �����Ʒ�������ٵ�0������ոò�λ
                if (item->getQuantity() == 0) {
                    itemsInSlot.clear();
                }
                return true; // �ɹ���������
            }
        }
    }
    return false; // �����Լ��ٻ��λ��Ч
}

// ��ȡָ����λ����Ʒ�б�
std::vector<Item*> Inventory::getItems(int row, int slot) const {
    if (row < ROWS && slot < SLOTS_PER_ROW) {
        return items[row][slot]; // ����ָ����λ����Ʒ�б�
    }
    return {}; // ���ؿ��б���ʾ��λ��Ч
}
void Inventory::selectSlot(int row, int col) {
    selectedSlot = { row, col }; // ��¼ѡ�в�λ������
}

bool Inventory::moveItems(int targetRow, int targetCol) {
    if (selectedSlot.first == -1 || selectedSlot.second == -1) {
        return false; // û��ѡ�в�λ
    }

    auto selectedItems = items[selectedSlot.first][selectedSlot.second];

    // ���Ŀ���λ�Ƿ�Ϊ��
    if (items[targetRow][targetCol].empty()) {
        items[targetRow][targetCol] = selectedItems;  // �ƶ���Ʒ
        items[selectedSlot.first][selectedSlot.second].clear(); // ���ԭ��λ
    }
    else {
        // ���н���
        std::swap(items[selectedSlot.first][selectedSlot.second], items[targetRow][targetCol]);
    }

    // ���ѡ��״̬
    selectedSlot = { -1, -1 };
    return true;
}