#include "SLRAnalyserTable.h"
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QDebug>

SLRAnalyserTable::SLRAnalyserTable()
{

}

SLRAnalyserTable::SLRAnalyserTable(Grammar G, ItemFamily itemFamily) {
    checkConflict(G, itemFamily);
    if(!isConflict()) {
        createSLRAnalyserTable(G, itemFamily);
    }
}

QList<QList<Lattice>> SLRAnalyserTable::getACTION() {
    return ACTION;
}

QList<QList<Lattice>> SLRAnalyserTable::getGOTO() {
    return GOTO;
}

QString SLRAnalyserTable::showConflict()
{
    if(!isConflict()) {
        return "这是一个SLR文法";
    } else {
        QString ss = "这不是一个SLR文法：\n";
        QList<int> keys = conflictMap.keys();
        for(int i=0; i<keys.size(); i++) {
            if(keys[i] == 0) {
                ss += "在项目集";
                for(int j=0; j<conflictMap.value(0).size(); j++) {
                    if(j == conflictMap.value(0).size() - 1) {
                        ss = ss + "I" + QString::number(conflictMap.value(0).at(j)) + "处存在移进-归约冲突\n";
                    } else {
                        ss = ss + "I" + QString::number(conflictMap.value(0).at(j)) + "、";
                    }

                }
            }
            if(keys[i] == 1) {
                ss += "在项目集";
                for(int j=0; j<conflictMap.value(1).size(); j++) {
                    if(j == conflictMap.value(1).size() - 1) {
                        ss = ss + "I" + QString::number(conflictMap.value(1).at(j)) + "处存在归约-归约冲突\n";
                    } else {
                        ss = ss + "I" + QString::number(conflictMap.value(1).at(j)) + "、";
                    }

                }
            }
        }
        return ss;
    }
}

void SLRAnalyserTable::checkConflict(Grammar G, ItemFamily itemFamily) {
    QList<Follow> follows = G.getFollows();
    QList<QString> terminal = G.getTerminals();
    QList<QString> nonTerminal = G.getNonTerminal();

    for(int i=0; i<itemFamily.itemSets.size(); i++) {
        ItemSet itemSet = itemFamily.itemSets[i];
        QList<QList<QString>> reduceItemFollows;

        for(int j=0; j<itemSet.items.size(); j++) {
            Item item = itemSet.items[j];
            if(item.getPoint() < item.rightParts.size()) {
                QString pointToken = item.rightParts[item.getPoint()];
                QList<QString> nonTerminalCaches;

                if(terminal.contains(pointToken)) { //点后面是非终结符X

                    //得到非终结符B 使得Follow(B)包含 a
                    for(int k=0; k<follows.size(); k++) {
                        if(follows[k].getFollow().contains(pointToken)) {
                            nonTerminalCaches.append(nonTerminal[k]);
                        }
                    }

                    //查看该项目集中是否有完整的项目B->y.
                    for(int k=0; k<itemSet.items.size(); k++) {
                        Item kitem = itemSet.items[k];
                        if(kitem.getPoint() >= kitem.rightParts.size()
                                && nonTerminalCaches.contains(kitem.leftPart)) {
                            conflictMap[0].append(i); //发生移进-归约冲突
                        }
                    }
                }
            } else {
                reduceItemFollows.append(follows[nonTerminal.indexOf(item.leftPart)].getFollow());
            }
        }

        //Follow(A) Follow(B)是否不相交
        for(int j=0; j<reduceItemFollows.size()-1; j++) {
            for(int k=j+1; k<reduceItemFollows.size(); k++) {
                if(intersect(reduceItemFollows[j], reduceItemFollows[k])) {
                    conflictMap[1].append(i); //发生归约-归约冲突
                }
            }
        }
    }
}

bool SLRAnalyserTable::isConflict()
{
    QList<int> keys = conflictMap.keys();
    if(keys.size() == 0)
        return false;
    return true;
}

/* 创建SLR分析表 */
void SLRAnalyserTable::createSLRAnalyserTable(Grammar G, ItemFamily itemFamily) {
    // 先把终结符和非终结符保存一下
    QList<QString> nonTerminal = G.getNonTerminal();
    QList<QString> terminal = G.getTerminals();
    QList<Follow> follows = G.getFollows();
    terminal.removeOne("ε");
    terminal.append("$");
//    nonTerminal.removeOne("START");

    // 为action表和goto表分配空间
    for(int i=0; i<itemFamily.itemSets.size(); i++) {
        // 为action表分配空间
        QList<Lattice> list1;
        for(int j=0; j<terminal.size(); j++) {
            Lattice lattice;
            list1.append(lattice);
        }

        // 为goto表分配空间
        QList<Lattice> list2;
        for(int j=0; j<nonTerminal.size(); j++) {
            Lattice lattice;
            list2.append(lattice);
        }

        ACTION.append(list1);
        GOTO.append(list2);
    }

    for(int i=0; i<itemFamily.itemSets.size(); i++) {
        ItemSet itemSet = itemFamily.itemSets[i];
        for(int j=0; j<itemSet.items.size(); j++) {
            Item item = itemSet.items[j];
            if(item.point < item.rightParts.size()) {  //点不在最后
                QString pointToken = item.rightParts[item.getPoint()];
                ItemSet gotoItemSet = goTo(itemSet, pointToken, itemFamily);

                if(terminal.contains(pointToken)
                        && itemSet.contains(item)) {

                    ACTION[i][terminal.indexOf(pointToken)].action = "s";
                    ACTION[i][terminal.indexOf(pointToken)].number = gotoItemSet.number;
                }

                if(nonTerminal.contains(pointToken)
                        && itemSet.contains(item)) {
                    GOTO[i][nonTerminal.indexOf(pointToken)].number = gotoItemSet.number;
                }


            } else {    //点在最后
                if(itemSet.contains(item) && item.leftPart != "START") {
                    for(int k=0; k<terminal.size(); k++) {
                        if(follows[nonTerminal.indexOf(item.leftPart)].getFollow().contains(terminal[k])) {
                            QString action = "r (";
//                            ACTION[i][k].action = "r";
                            ACTION[i][k].number = item.number;
                            for(int k=0; k<G.getProductions().size(); k++) {
                                Production prod = G.getProductions()[k];
                                if(prod.number == item.number) {
                                    action = action + prod.leftPart + "->";
                                    for(int l=0; l<prod.rightParts.size(); l++) {
                                        action = action + prod.rightParts[l] + " ";
                                    }
                                    action += ")";
                                    break;
                                }
                            }
                            ACTION[i][k].action = action;
                        }
                    }
                }

                if(itemSet.contains(itemFamily.itemSets[1].items[0])) {
                    ACTION[i][terminal.indexOf("$")].action = "acc";
                }

            }

        }
    }

    if(nonTerminal.contains("START")) {
        for(int i=0; i<GOTO.size(); i++) {
            GOTO[i].removeFirst();
        }
    }

}

void SLRAnalyserTable::showSLRTable(Grammar G)
{
    QString error = "ERROR";

    QTableView *SLRTable = new QTableView;
    SLRTable->setWindowTitle("SLR(1)分析表");
    SLRTable->verticalHeader()->hide();
    SLRTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SLRTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(240, 240, 240);}");
//    SLRTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    SLRTable->resizeColumnsToContents();
    SLRTable->resize(750, 500);

    QStandardItemModel* model = new QStandardItemModel();   //创建数据模型
    model->setHorizontalHeaderLabels(getTitleList(G)); //设置表格标题行

    /* 填充表格内容 */
    for(int i=0; i<ACTION.size(); i++) {
        model->setItem(i, 0, new QStandardItem(QString::number(i)));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);

        for(int j=0; j<ACTION[i].size(); j++) {
            if(ACTION[i][j].number != 0 || ACTION[i][j].action[0] == "a") {
                if(ACTION[i][j].action[0] == "r" || ACTION[i][j].action[0] == "a") {
                    model->setItem(i, j + 1,
                                   new QStandardItem(ACTION[i][j].action));
                } else {
                    model->setItem(i, j + 1,
                                   new QStandardItem(ACTION[i][j].action + QString::number(ACTION[i][j].number)));

                }
                model->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);

            } else {
//                model->setItem(i, j + 1,
//                               new QStandardItem(" "));
            }

        }

        for(int j=0; j<GOTO[i].size(); j++) {
            if(GOTO[i][j].number != 0) {
                model->setItem(i, ACTION[i].size() + j + 1,
                               new QStandardItem(QString::number(GOTO[i][j].number)));
                model->item(i, ACTION[i].size() + j + 1)->setTextAlignment(Qt::AlignCenter);
            } else {
//                model->setItem(i, ACTION[i].size() + j + 1,
//                               new QStandardItem(" "));
            }



        }
    }

    /* 设置表格视图数据并展示 */
    SLRTable->setModel(model);
    SLRTable->show();
}

QStringList SLRAnalyserTable::getTitleList(Grammar G)
{
    QList<QString> list;
    QList<QString> nonTerminal = G.getNonTerminal();
    QList<QString> terminal = G.getTerminals();
    terminal.append("$");
    terminal.removeOne("ε");
    nonTerminal.removeOne("START");

    list.append("");

    for(int i=0; i<terminal.size(); i++) {
        list.append(terminal[i]);
    }

    for(int i=0; i<nonTerminal.size(); i++) {
        list.append(nonTerminal[i]);
    }

    return list;
}

bool SLRAnalyserTable::intersect(QList<QString> follow1, QList<QString> follow2)
{
    for(int i=0; i<follow1.size(); i++) {
        for(int j=0; j<follow2.size(); j++) {
            if(follow2[j] == follow1[i]) {
                return true;
            }
        }
    }
    return false;
}

ItemSet SLRAnalyserTable::goTo(ItemSet itemSet, QString grammarSymbol, ItemFamily itemFamily)
{
    ItemSet i;
    for(int i=0; i<itemFamily.itemSets.size(); i++) {
        if(itemFamily.DFA[itemSet.number][i] == grammarSymbol) {
            for(int j=0; j<itemFamily.itemSets.size(); j++) {
                if(itemFamily.itemSets[i].number == i) {
                    return itemFamily.itemSets[i];
                }
            }
        }
    }
    return i;
}
