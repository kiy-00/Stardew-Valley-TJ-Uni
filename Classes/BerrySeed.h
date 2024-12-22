#ifndef __BERRY_SEED_H__
#define __BERRY_SEED_H__

#include "Item.h"
#include "Berry.h"
#include "FarmlandTile.h"

class BerrySeed : public Item {
public:
    BerrySeed() : Item(
        "Berry Seed",                          // ����
        "A seed that grows into sweet berries", // ����
        "seed",                                // ��Ʒ����
        "crops/berry/berry_0.png",          // ͼƬ·��
        1                                      // ��ʼ����
    ) {}

    static BerrySeed* create() {
        return new BerrySeed();
    }
};

#endif // __BERRY_SEED_H__