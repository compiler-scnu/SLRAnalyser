#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <QList>
#include "Production.h"
#include "First.h"
#include "Follow.h"
#include <QException>
#include <QFile>


/**
 * @brief The Grammar class
 */
class Grammar
{
public:
    Grammar();
    Grammar(QString);
    QList<QString> getNonTerminal();
    QList<QString> getTerminals();
    QList<Production> getProductions();
    QList<Follow> getFollows();
    QList<First> getFirsts();

private:
    QList<Production> productions; // 产生式列表，保存了多条产生式后就叫做文法
    QList<First> firsts; // 文法的First集
    QList<Follow> follows; // 文法的Follow集
    QList<QString> terminals;
    QList<QString> nonTerminals;

    void createNonTerminals(); //根据文法，生成非终结符集合
    void createTerminals(); //根据文法，生成终结符集合
    void createFirst(); //根据终结符和非终结符还有文法生成first集
    void createFollow(); //根据终结符和非终结符还有文法生成follow集
    First getFirsts(Production, int);
};

#endif // GRAMMAR_H
