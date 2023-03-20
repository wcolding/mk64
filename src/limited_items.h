#ifndef LIMITED_ITEMS_H
#define LIMITED_ITEMS_H

#include "math_util.h"
#include "common_structs.h"

struct itemsList 
{
    u8 length;
    u8 items[15];
};

struct itemsList gItemsArray;
u16 gItemsUnlocked = 0x6FFF;

void updateItemsList(u16 mask)
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
        if ((mask & flag) != 0) 
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

s32 getLimitedItem(Player *p) {
    u16 random;
    u16 adjustedItems = gItemsUnlocked;
    
    // If player is in first, limit items further
    if (p->currentRank == 0) {
        adjustedItems = 0x0001; // bananas only
    }
    else if (p->currentRank < 4) {
        adjustedItems |= 0x04C0;
        adjustedItems ^= 0x04C0; // no lightning, blue shells or boo above 4th
    }

    if (adjustedItems == 0) {
        return 0;
    }

    updateItemsList(adjustedItems);
    random = random_int(gItemsArray.length);
    return gItemsArray.items[random];

    return 0;
}



#endif