#include <QFileDialog>
#include <QMessageBox>
#include <QException>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SLRAnalyserTable.h"
#include "Grammar.h"
#include "ItemFamily.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveGrammar()
{
    QString grammar = ui->grammarView->toPlainText();

    QFile file(grammarPath);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        file.write(grammar.toStdString().data());

        file.close();
    }
}

/* 展示Follow集合 */
void MainWindow::on_showFollowBtn_clicked()
{
    ui->showView->clear();
    saveGrammar();

    Grammar G(grammarPath);

    for(int i=0; i<G.getFollows().size(); i++) {  
        QString follow;
        follow = "Follow(" + G.getNonTerminal()[i] + ") = { ";

        for(int j=0; j<G.getFollows()[i].getFollow().size(); j++) {

            if(j == G.getFollows()[i].getFollow().size() - 1) {
                follow += G.getFollows()[i].getFollow()[j] + " }";
            } else {
                follow += G.getFollows()[i].getFollow()[j] + ", ";
            }
        }

        ui->showView->append(follow);
    }
}

/* 展示First集合 */
void MainWindow::on_showFirstBtn_clicked()
{
    ui->showView->clear();
    saveGrammar();

    Grammar G(grammarPath);

    for(int i=0; i<G.getFirsts().size(); i++) {
        QString first;
        first = "First(" + G.getNonTerminal()[i] + ") = { ";

        for(int j=0; j<G.getFirsts()[i].getFirst().size(); j++) {
            if(j == G.getFirsts()[i].getFirst().size() - 1) {
                first += G.getFirsts()[i].getFirst()[j] + " }";
            } else {
                first += G.getFirsts()[i].getFirst()[j] + ", ";
            }
        }
        ui->showView->append(first);
    }
}

/* 打开文件 */
void MainWindow::on_openFileBtn_clicked()
{
    ui->grammarView->clear();
    ui->showView->clear();

    QString fileName = QFileDialog::getOpenFileName(this, NULL, "./TEST", "*.txt");

    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!file.atEnd()) {
            QString line = file.readLine();
            if(line[line.size() - 1] == "\n") {
                line.remove(line.size() - 1, 1);
            }
            ui->grammarView->append(line);
        }
    } else {
        QMessageBox ::information(NULL, NULL, "open file error");
    }
}

/* 展示DFA状态 */
void MainWindow::on_showLRDFABtn_clicked()
{
    ui->showView->clear();
    saveGrammar();

    Grammar G(grammarPath);
    if(G.getFirsts().size() != 0) {
        ItemFamily itemFamily(G);

        QList<ItemSet> itemSets = itemFamily.itemSets;

        ui->showView->append("该文法的项目集规范族如下所示：");
        for(int i=0; i<itemSets.size(); i++) {
            QString Ii = "I" + QString::number(i) + ":";
            ui->showView->append(Ii);
            for(int j=0; j<itemSets[i].items.size(); j++) {
                QString s = itemSets[i].items[j].leftPart;
                s += "->";
                for(int k=0; k<itemSets[i].items[j].rightParts.size(); k++) {
                    if(itemSets[i].items[j].point == k) {
                        s += ".";
                    }
                    s = s + itemSets[i].items[j].rightParts[k] + " ";
                }
                if(itemSets[i].items[j].point == itemSets[i].items[j].rightParts.size()) {
                    s += ".";
                }
                ui->showView->append(s);
            }
        }

        ui->showView->append("该文法的DFA如下所示：");
        for(int i=0; i<itemSets.size(); i++) {
            for (int j=0; j<itemSets.size(); j++) {
                if(itemFamily.DFA[i][j] != "") {
                    QString edge = "I" + QString::number(i)+ "\t" + itemFamily.DFA[i][j] + "\tI" + QString::number(j);
                    ui->showView->append(edge);
                }
            }
        }
    }

}

/* 查看SLR分析表 */
void MainWindow::on_showSLRTableBtn_clicked()
{
    ui->showView->clear();
    saveGrammar();

    Grammar G(grammarPath);

    if(G.getFirsts().size() != 0) {
        ItemFamily itemFamily(G);
        SLRAnalyserTable slrTable(G, itemFamily);
        if(slrTable.isConflict()) {
            QMessageBox::information(NULL, NULL, "这不是一个SLR文法");
        } else {
            slrTable.showSLRTable(G);
        }
    }
}

/* 检查是否为SLR文法 */
void MainWindow::on_checkSLRBtn_clicked()
{
    ui->showView->clear();
    saveGrammar();

    Grammar G(grammarPath);

    if(G.getFirsts().size() != 0) {
        ItemFamily itemFamily(G);
        SLRAnalyserTable slrTable(G, itemFamily);

        ui->showView->append(slrTable.showConflict());
    }

}

void MainWindow::on_saveFileBtn_clicked()
{
    ui->showView->clear();
    QString grammar = ui->grammarView->toPlainText();
    if(grammar.isEmpty())
    {
        QMessageBox ::information(NULL, NULL, "没有文法，保存失败！");
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存当前文法"),
                                                        "",
                                                        tr("文法文件(*.txt)"));
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::critical(this, "critical", tr("文件保存失败！"),
                                  QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            file.write(grammar.toStdString().data());

            file.close();
        }
    }
}
