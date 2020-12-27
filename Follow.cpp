#include "Follow.h"

Follow::Follow()
{

}

QList<QString> Follow::getFollow()
{
    return follow;
}

bool Follow::setFollow(Follow nonTerminal)
{
    bool judge = false;
    for(int i=0; i<nonTerminal.getFollow().size(); i++)
    {
        if(!follow.contains(nonTerminal.getFollow()[i]) && nonTerminal.getFollow()[i] != "ε")
        {
            //如果follow集中不包含对象非终结符中的第i个元素，那么就添加
            follow.append(nonTerminal.getFollow()[i]);
            judge = true;
        }
    }
    return judge;
}

bool Follow::setFollow(First nonTerminal)
{
    bool judge = false;
    for(int i=0; i<nonTerminal.getFirst().size(); i++)
    {
        if(!follow.contains(nonTerminal.getFirst()[i]) && nonTerminal.getFirst()[i] != "ε")
        {
            //如果Follow集中不包含对象非终结符中的第i个元素，那么就添加
            follow.append(nonTerminal.getFirst()[i]);
            judge = true;
        }
    }
    return judge;
}

bool Follow::setFollow(QString terminal)
{
    bool judge = false;
    if(!follow.contains(terminal) && terminal != "ε")
    {
        //如果Follow集中不包含该终结符，则将这个终结符添加到Follow集中
        follow.append(terminal);
        judge = true;
    }
    return judge;
}
