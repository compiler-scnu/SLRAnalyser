#ifndef SLRANALYSERTABLE_H
#define SLRANALYSERTABLE_H

#include <QList>
#include <QMap>
#include "Lattice.h"
#include "Grammar.h"
#include "ItemFamily.h"

class SLRAnalyserTable
{
public:
    SLRAnalyserTable();
    SLRAnalyserTable(Grammar, ItemFamily);
    QList<QList<Lattice>> getACTION();
    QList<QList<Lattice>> getGOTO();
    bool isConflict();
    void showSLRTable(Grammar);
    QString showConflict();

private:
    QList<QList<Lattice>> ACTION;
    QList<QList<Lattice>> GOTO;
    /**
      key为冲突类型   0：不冲突   1：移进-归约冲突   2：归约-归约冲突
      value为发生冲突的位置。
     * @brief conflictMap
     */
    QMap<int, QList<int>> conflictMap;

    void createSLRAnalyserTable(Grammar, ItemFamily);
    QStringList getTitleList(Grammar); 
    ItemSet goTo(ItemSet, QString, ItemFamily);
    void checkConflict(Grammar, ItemFamily);
    bool intersect(QList<QString>, QList<QString>);
};

#endif // SLRANALYSERTABLE_H
