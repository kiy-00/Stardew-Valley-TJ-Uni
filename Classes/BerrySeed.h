#ifndef __BERRY_SEED_H__
#define __BERRY_SEED_H__

#include "Item.h"

class BerrySeed : public Item {
public:
    BerrySeed() : Item(
        "Berry Seed",                          // ����
        "A seed that grows into sweet berries", // ����
        "seed",                                // ��Ʒ����
        "crops/berry/berry_0.png",          // ͼƬ·��
        1                                      // ��ʼ����
    ) {}

    //δ�����ĳ�ʼ�����߼�
    static BerrySeed* create(int quantity = 50) {
        BerrySeed* seed = new BerrySeed();
        seed->setQuantity(quantity);  // ���ó�ʼ����
        return seed;
    }
};

#endif // __BERRY_SEED_H__