#ifndef LIMITED_ITEMS_H
#define LIMITED_ITEMS_H

#include "math_util.h"

struct itemsList 
{
    u8 length;
    u8 items[15];
};

struct itemsList gItemsArray;
u16 gItemsUnlocked = 0x7FFF;

void updateItemsList(void)
{
    int i;
    int flag = 0;

    gItemsArray.length = 0;
    for (i = 0; i < 15; i++)
    {
        gItemsArray.items[i] = 0;
    }

    for (i = 0; i < 15; i++)
    {
        flag = (1 << i);
        if ((gItemsUnlocked & flag) != 0) 
        {
            gItemsArray.items[gItemsArray.length] = i + 1;
            gItemsArray.length++;
        }
    }
}

s32 getRandomItem(void) 
{
    u16 random = random_int(15);
    return random;
}

s32 getLimitedItem(void) {
    u16 random;

    if (gItemsUnlocked == 0) {
        return 0;
    }

    updateItemsList();
    random = random_int(gItemsArray.length);
    return gItemsArray.items[random];

    return 0;
}



#endif