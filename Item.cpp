#include "Item.h"
#include <QDebug>
Item::Item()
{

}

Item::Item(const Item &item) : Production(item)
{
    this->point = item.point;
    this->leftPart = item.leftPart;
    this->rightParts = item.rightParts;
    this->number = item.number;
}

Item::Item(Production production)
{
    leftPart = production.leftPart;
    if(production.rightParts.contains("ε")) {
        production.rightParts.removeAt(production.rightParts.indexOf("ε"));
    }
    rightParts = production.rightParts;
    this->number = production.number;
}

int Item::getPoint()
{
    return point;
}

bool Item::productionEquals(Item item)
{
    bool judge = false;
    if(rightParts.size() == item.rightParts.size()) {
        int i = 0;
        for(i=0; i<rightParts.size(); i++) {
            if(rightParts[i] != item.rightParts[i]) {
                break;
            }
        }

        if(i == rightParts.size()) {
            judge = true;
        }

        if(judge && leftPart == item.leftPart) {
            return true;
        }
    }
    return false;
}
