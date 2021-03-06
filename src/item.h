#ifndef ITEM_H
#define ITEM_H

/**
 * \file item.h
 * \author groupe Larakis
 * \version 1
 * \date 20/02/2016
 * \brief Headers of item file
 * \details All header of the item file functions
 */

#include <stdbool.h>
#include "enum.h"

typedef struct Item* Item;
/**
 * \struct Item
 * \brief The item structure
 * \details
 **/

struct Item
{
    int posX;
    int posY;
    BoardValue value;
    //void (*onCollision)(Item i, Snake sOnCollision, Snake sBis); // idSnake:id du snake qui a touché l'object
    Item prev;
    Item next;
};

/*** Object ***/
Item itemCreate(int x, int y, BoardValue value);
Item itemAddNew(Item list, int x, int y, BoardValue value);
Item itemAdd(Item list, Item toAdd);
BoardValue itemGetRandomItemValue();
Item itemSearch(Item list, int x, int y);
Item itemSearchByValue(Item list, BoardValue value);
bool itemListIsEmpty(Item list);
int itemDelete(Item item);
void itemFree(Item item);

#endif
