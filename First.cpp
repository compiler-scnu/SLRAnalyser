#include "First.h"

First::First()
{

}

QList<QString> First::getFirst()
{
    return first;
}

bool First::setFirst(First nonTerminal)
{
    bool judge = false;
    for (int i=0; i<nonTerminal.getFirst().size(); i++)
    {
        if(!first.contains(nonTerminal.getFirst()[i]) && nonTerminal.getFirst()[i] != "ε")
        {
            //如果first集中不包含对象非终结符中的第i个元素，那么就添加
            first.append(nonTerminal.getFirst()[i]);
            judge = true;
        }
    }
    return judge;
}

bool First::setFirst(QString terminal)
{
    if(!first.contains(terminal)) {
        first.append(terminal);
        return true;
    }
    return false;
}
