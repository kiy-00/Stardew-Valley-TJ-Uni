#ifndef __WHEAT_SEED_H__
#define __WHEAT_SEED_H__

#include "Item.h"

class WheatSeed : public Item {
public:
    WheatSeed() : Item(
        "Wheat Seed",                          // ����
        "A seed that grows into wheats, which is a important crop", // ����
        "seed",                                // ��Ʒ����
        "crops/wheat/wheat_0.png",          // ͼƬ·��
        1                                      // ��ʼ����
    ) {
    }

    //δ�����ĳ�ʼ�����߼�
    static WheatSeed* create(int quantity = 50) {
        WheatSeed* seed = new WheatSeed();
        seed->setQuantity(quantity);  // ���ó�ʼ����
        return seed;
    }
};

#endif // __CARROT_SEED_H__