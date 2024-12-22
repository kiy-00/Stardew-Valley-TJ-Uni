#ifndef __PEPPER_SEED_H__
#define __PEPPER_SEED_H__

#include "Item.h"

class PepperSeed : public Item {
public:
    PepperSeed() : Item(
        "Pepper Seed",                          // ����
        "A seed that grows into pepper, which programmer don't like", // ����
        "seed",                                // ��Ʒ����
        "crops/pepper/pepper_0.png",          // ͼƬ·��
        1                                      // ��ʼ����
    ) {
    }

    //δ�����ĳ�ʼ�����߼�
    static PepperSeed* create(int quantity = 50) {
        PepperSeed* seed = new PepperSeed();
        seed->setQuantity(quantity);  // ���ó�ʼ����
        return seed;
    }
};

#endif // __CARROT_SEED_H__