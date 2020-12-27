#ifndef ITEMFAMILY_H
#define ITEMFAMILY_H

#include <QList>
#include "Itemset.h"
#include "Grammar.h"

const int maxSize = 100;

class ItemFamily
{
public:
    QList<ItemSet> itemSets;
    QString DFA[maxSize][maxSize];

    ItemFamily();
    ItemFamily(Grammar);


private:
    void initFamily(Grammar);
    void createFamily(Grammar);
    bool equals(ItemSet, ItemSet);

};

#endif // ITEMFAMILY_H
