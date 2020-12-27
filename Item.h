#ifndef ITEM_H
#define ITEM_H

#include "Production.h"

/**
*  项目集规范族中的其中一个项目的其中一个产生式
 * @brief The Item class
 */
class Item: public Production
{
public:
    int point = 0; // 假设point=0，则表示E'->E

    Item();
    Item(const Item &item);
    Item(Production);
    bool productionEquals(Item);
    int getPoint();

private:

};

#endif // ITEM_H
