#include "Grammar.h"
#include <QDebug>
Grammar::Grammar()
{

}

Grammar::Grammar(QString path)
{
    int number = 0;
    QString prods = "";
    bool judge = true;

    QFile grammarFile(path);
    QFile productionFile("./TRANSIT/production.txt");

    if(grammarFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if(grammarFile.atEnd()) {
            if(productionFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
                productionFile.write(prods.toStdString().data());

                productionFile.close();
            }
        }

        while (!grammarFile.atEnd()) {

            QByteArray line = grammarFile.readLine();
            QString strline = line;


            QStringList list = strline.split("->");

            /* 扩充文法 */
            if(judge && strline.contains('|')) {
                prods = "START->" + list[0] + "\n";
            }
            judge = false;

            QStringList rightList = list[1].split("|");
            for(int i=0; i<rightList.size(); i++) {
                QString prod = list[0] + "->" + rightList[i];
                if(prod[prod.size() - 1] != "\n") {
                    prod += "\n";
                }
                prods += prod;
            }

            if(productionFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
                productionFile.write(prods.toStdString().data());

                productionFile.close();
            }
        }
        grammarFile.close();
    }

    if(productionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!productionFile.atEnd()) {
            QByteArray line = productionFile.readLine();

            if (line != nullptr) {
                number++;
                Production production(line);
                production.number = number;
                productions.append(production);
            }
        }
        productionFile.close();
    }

    createNonTerminals(); // 根据文法，计算非终结符
    createTerminals(); // 根据文法，计算终结符
    createFirst(); // 根据文法，计算First集
    createFollow(); // 根据文法，计算Follow集
}

QList<Production> Grammar::getProductions()
{
    return productions;
}

QList<QString> Grammar::getNonTerminal()
{
    return nonTerminals;
}

QList<QString> Grammar::getTerminals()
{
    return terminals;
}

QList<Follow> Grammar::getFollows()
{
    return follows;
}

QList<First> Grammar::getFirsts()
{
    return firsts;
}

/** 根据文法，生成非终结符集合 */
void Grammar::createNonTerminals()
{
    int index = 0;
    while(index < productions.size())
    {
        //indexString：定义一个中间变量字符串，用来保存第index条产生式的左部
        QString indexString = productions[index].leftPart;
        if(!nonTerminals.contains(indexString))
        {
            nonTerminals.append(indexString);
        }
        index++;
    }
}

/** 根据文法，生成终结符集合 */
void Grammar::createTerminals()
{
    int index = 0;
    while(index < productions.size())
    {
        int temp = 0;
        while(temp < productions[index].rightParts.size())
        {
            // indexString：定义一个中间变量字符串，用来保存第index条产生式的右部的第temp个符号
            QString indexString = productions[index].rightParts[temp];
            if(!nonTerminals.contains(indexString) && !terminals.contains(indexString))
            {
                // 如果不在非终结符集合中，没有被添加到终结符集合，就将该终结符保存
                terminals.append(indexString);
            }
            temp++;
        }
        index++;
    }
}

/** 就是根据终结符和非终结符还有文法生成first集 */
void Grammar::createFirst()
{
    bool _ALTERED_ = true; //判断是否对First集进行过修改

    /* 让每一个非终结符的first集合都为空 */
    for(int i=0; i<nonTerminals.size(); i++) {
        First first;
        firsts.append(first);
    }

    /* /让终结符的first集合为它自身 */
    for(int i=0; i<terminals.size(); i++) {
        First first;
        first.setFirst(terminals[i]);
        firsts.append(first);
    }

    while (_ALTERED_) {
        _ALTERED_ = false;

        for(int i=0; i<productions.size(); i++) {
            int k = 0;
            bool Continue = true;
            QString leftPart = productions[i].leftPart;
            QList<QString> rightParts = productions[i].rightParts;
            int leftPartLocation = nonTerminals.indexOf(leftPart);

            while (Continue && k<rightParts.size()) {

                int location;
                if(nonTerminals.contains(rightParts[k])) {
                    location = nonTerminals.indexOf(rightParts[k]);

                    if(firsts[leftPartLocation].setFirst(firsts[location])) {
                        _ALTERED_ = true;
                    }

                } else {
                    location = terminals.indexOf(rightParts[k]) + nonTerminals.size();

                    if(firsts[leftPartLocation].setFirst(firsts[location])) {
                        _ALTERED_ = true;
                    }

                }

                if(!firsts[location].getFirst().contains("ε")) {
                    Continue = false;
                }
                k++;
            }

            if(Continue) {
                if(firsts[leftPartLocation].setFirst("ε")) {
                    _ALTERED_ = true;
                }
            }
        }
    }

//    for(int i=0; i<terminals.size(); i++) {
//        firsts.removeLast();
//    }

}

/** 就是根据终结符和非终结符还有文法生成follow集 */
void Grammar::createFollow()
{
    bool _ALTERED_ = true; // 判断是否对Follow集进行过修改

    for(int i=0; i<nonTerminals.size(); i++)
    {
        Follow follow;
        if(i == 0) {
            follow.setFollow("$");
        }
        follows.append(follow);
    }

    while (_ALTERED_) {
        _ALTERED_ = false;

        for(int i=0; i<productions.size(); i++) {
            QList<QString> rightParts = productions[i].rightParts;
            QString leftPart = productions[i].leftPart;

            for(int k=0; k<rightParts.size(); k++) {

                int location = nonTerminals.indexOf(rightParts[k]);

                if(nonTerminals.contains(rightParts[k])) {
                    if(follows[location].setFollow(getFirsts(productions[i], k))) {
                        _ALTERED_ = true;
                    }

                    if(getFirsts(productions[i], k).getFirst().contains("ε")) {
                        if(follows[location].setFollow(follows[nonTerminals.indexOf(leftPart)])) {
                            _ALTERED_ = true;
                        }
                    }
                }


            }
        }
    }

    for(int i=0; i<terminals.size(); i++) {
        firsts.removeLast();
    }
}

First Grammar::getFirsts(Production prod, int index)
{
    First first;
    QList<QString> rightParts = prod.rightParts;
    int i;

    for(i=index+1; i<rightParts.size(); i++) {
        int location;
        if(terminals.contains(rightParts[i])) {
            location = terminals.indexOf(rightParts[i]) + nonTerminals.size();

        } else {
            location = nonTerminals.indexOf(rightParts[i]);

        }
        first.setFirst(firsts[location]);
        if(!firsts[location].getFirst().contains("ε")) {
            break;
        }

    }
    if(i == rightParts.size()) {
        first.setFirst("ε");
    }
    return first;
}
