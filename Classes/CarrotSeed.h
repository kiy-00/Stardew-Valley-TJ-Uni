#ifndef __CARROT_SEED_H__
#define __CARROT_SEED_H__

#include "Item.h"

class CarrotSeed : public Item {
public:
    CarrotSeed() : Item(
        "Carrot Seed",                          // ����
        "A seed that grows into health carrots", // ����
        "seed",                                // ��Ʒ����
        "crops/carrot/carrot_0.png",          // ͼƬ·��
        1                                      // ��ʼ����
    ) {
    }

    //δ�����ĳ�ʼ�����߼�
    static CarrotSeed* create(int quantity = 50) {
        CarrotSeed* seed = new CarrotSeed();
        seed->setQuantity(quantity);  // ���ó�ʼ����
        return seed;
    }
};

#endif // __CARROT_SEED_H__