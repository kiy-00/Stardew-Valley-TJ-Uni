#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <vector>
#include "Item.h"

const int ROWS = 3;        // ����
const int SLOTS_PER_ROW = 8; // ÿ�в�λ��
const int MAX_ITEMS_PER_SLOT = 5; // ÿ����λ���ŵ���Ʒ��

class Inventory : public cocos2d::Node {
public:
    Inventory(); // ���캯������
    ~Inventory(); // ������������

    bool addItem(Item* item); // ���ز���ֵָʾ�Ƿ���ӳɹ�
    bool reduceItemQuantity(int row, int slot, int quantity);
    std::vector<Item*> getItems(int row, int slot) const; // ��ȡָ����λ����Ʒ�б�
    void selectSlot(int row, int col);
    bool Inventory::moveItems(int targetRow, int targetCol);
  

private:
    std::vector<Item*> items[ROWS][SLOTS_PER_ROW]; // �洢��Ʒ�Ķ�ά����
    std::pair<int, int> selectedSlot = { -1, -1 };// ѡ�в�λ������
};

#endif