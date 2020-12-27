#include "Itemset.h"

ItemSet::ItemSet()
{

}

void ItemSet::initItemSet(Item item, Grammar G)
{
    bool _ALTERED_ = true;
    int i = items.size();
    items.append(item);
    QList<QString> nonTerminal = G.getNonTerminal();

    while (_ALTERED_) {
        if(item.point >= item.rightParts.size()) {
            break;
        }

        _ALTERED_ = false;
        int items_size = items.size();

        for(; i<items_size; i++) {
            Item iitem = items[i];
            if(iitem.getRightParts().size() == 0) {
                continue;
            }
            QString pointToken = iitem.getRightParts()[iitem.getPoint()];
            if(nonTerminal.contains(pointToken)) {
                for(int k=0; k<G.getProductions().size(); k++) {
                    Production production = G.getProductions()[k];
                    if(pointToken == production.leftPart) {
                        Item newItem(production);
                        if(!contains(newItem)) {
                            items.append(newItem);
                            _ALTERED_ = true;
                        }
                    }
                }
            }
        }

    }
}

bool ItemSet::contains(Item item)
{
    // List自带的contains好像不可以用，判断items中是否有item
    int i = 0;
    for(; i<items.size(); i++)
    {
        // 判断第i个元素和传入的元素是否相同
        if(equals(items[i], item)) {
            break;
        }
    }
    if(i >= items.size()) {
        return false;
    } else {
        return true;
    }
}

bool ItemSet::equals(Item item1, Item item2)
{
    // 判断item1和item2是否相同
    bool judge = false;
    if(item1.rightParts.size() == item2.getRightParts().size()) {
        int j = 0;
        for(; j<item1.rightParts.size(); j++) {
            if(item1.getRightParts()[j] != item2.getRightParts()[j]) {
                break;
            }
        }
        if(j >= item1.rightParts.size()) {
            // 右部相等
            judge = true;
        }
    }
    if(judge && item1.leftPart == item2.leftPart && item1.point == item2.point) {
        // 如果右部相同，且左部和点的位置都相同，那么说明就是一样的
        return true;
    } else {
        return false;
    }
}
