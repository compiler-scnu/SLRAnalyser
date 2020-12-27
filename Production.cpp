#include "Production.h"

Production::Production()
{

}

Production::Production(QString s)
{
    Separate(s);
}

void Production::Separate(QString s)
{
    bool left = true;
    int i = 0;
    while (i < s.length()) //如果没有读到字符串的末尾，就继续循环判断
    {
        bool special = true; //产生式分析的时候遇到非字母的元素，就比如是遇到了!@#$%^&*()之类的元素
        if(s[i] == '-' && s[i + 1] == '>')
        {
            //如果遇到了->标志，就说明是到了产生式右部了，置标志位为false，表示开始记录右部
            i += 2;
            left = false;
            continue;
        }

        QString indexString = ""; //初始化一个用于暂时存放文法中单词的字符串变量
        while((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '\'')
        {
            //如果遇到的字符是字母，就将这个字符保存在indexString中，并继续判断下一个字符是不是字符
            special = false;
            indexString += s[i++];
            if(i == s.length())
                break;
        }

        if(special) {
            if(s[i] == ' ') {
                i++;
                continue;
            } else {
                indexString += s[i++];
            }
        }

        if(indexString != "\n") {
            if(left) {
                //如果程序此时正在判断产生式的左部，那么就将中间变量indexString保存在产生式左部字符串变量中
                leftPart = indexString;
            } else {
                rightParts.append(indexString);
            }
        }
    }
}

QString Production::getLeftPart()
{
    return leftPart;
}

void Production::setLeftPart(QString leftPart)
{
    this->leftPart = leftPart;
}

QList<QString> Production::getRightParts()
{
    return rightParts;
}

void Production::setRightParts(QList<QString> rightParts)
{
    this->rightParts = rightParts;
}
