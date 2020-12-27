#include "ItemFamily.h"

ItemFamily::ItemFamily()
{

}

ItemFamily::ItemFamily(Grammar G)
{
    initFamily(G);
    createFamily(G);
}

void ItemFamily::initFamily(Grammar G)
{
    Production _START_ = G.getProductions()[0];
    Item item(_START_); // 将产生式形式的拓广文法用项目形式保存，类似START->·E
    ItemSet is;
    is.initItemSet(item, G);
    is.number = 0;
    itemSets.append(is); // 根据拓广文法产生的项目集I0，将I0添加到项目集规范族中
}

void ItemFamily::createFamily(Grammar G)
{
    int number = 1;

    for(int i=0; i<itemSets.size(); i++) {
        QList<QString> caches;
        ItemSet itemSet = itemSets[i];

        for(int k=0; k<itemSet.items.size(); k++) {
            Item item = itemSet.items[k];

            /* 只有一个点或者是规约项 如: S->. S'->S. */
            if(item.getPoint() >= item.getRightParts().size()) {
                continue;
            }

            if(!caches.contains(item.rightParts[item.getPoint()])) {
                caches.append(item.rightParts[item.getPoint()]);
            }
        }

        for(int j=0; j<caches.size(); j++) {
            QString cache = caches[j];
            ItemSet newItemSet;

            for(int k=0; k<itemSet.items.size(); k++) {
                Item item = itemSet.items[k];

                if(item.getPoint() < item.rightParts.size()) {
                    if(item.rightParts[item.getPoint()] == cache) {
                        int point = item.getPoint() + 1;
                        Item newItem(item);
                        newItem.point = point;

                        newItemSet.initItemSet(newItem, G);
                    }

                }
            }

            int l = 0;
            for(l=0; l<itemSets.size(); l++) {
                if(equals(newItemSet, itemSets[l])) {
                    /* 构造dfa */
                    DFA[itemSet.number][itemSets[l].number] = cache;
                    break;
                }
            }
            if(l >= itemSets.size()) {
                newItemSet.number = number++;
                itemSets.append(newItemSet);

                /* 构建dfa */
                DFA[itemSet.number][newItemSet.number] = cache;
            }

        }
    }
}

bool ItemFamily::equals(ItemSet itemSet1, ItemSet itemSet2)
{
    if(itemSet1.items.size() != itemSet2.items.size()) {
        return false;
    }
    for(int i=0; i<itemSet1.items.size(); i++) {
        int j = 0;
        for(; j<itemSet2.items.size(); j++) {
            if(itemSet1.equals(itemSet1.items[i], itemSet2.items[j])) {
                break;
            }
        }
        if(j == itemSet2.items.size()) {
            return false;
        }
    }
    return true;
}
