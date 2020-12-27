#ifndef ITEMSET_H
#define ITEMSET_H

#include <QList>
#include "Item.h"
#include "Grammar.h"

class ItemSet
{
public:
    int number;
    QList<Item> items;

    ItemSet();
    void initItemSet(Item, Grammar);
    bool equals(Item, Item);
    bool contains(Item);

private:


};

#endif // ITEMSET_H
