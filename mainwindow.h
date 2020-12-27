#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_showFollowBtn_clicked();

    void on_showFirstBtn_clicked();

    void on_openFileBtn_clicked();

    void on_showLRDFABtn_clicked();

    void on_showSLRTableBtn_clicked();

    void on_checkSLRBtn_clicked();

    void on_saveFileBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString grammarPath = "./TRANSIT/grammar.txt";

    void saveGrammar();
};
#endif // MAINWINDOW_H
