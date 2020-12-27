#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QList>

/**
*  产生式类，这里规定了文法中的每一条产生式都是由字符组成的。如果出现了诸如“_F”形式的终结符或非终结符，都将不被识别。
*  并且程序会自动判断，产生式左边的一定是非终结符。
 * @brief The Production class
 */
class Production
{
public:
    int number;
    QString leftPart = "";
    QList<QString> rightParts;

    Production();
    Production(QString);
    QString getLeftPart();
    QList<QString> getRightParts();
    void setLeftPart(QString);
    void setRightParts(QList<QString>);

private:
    void Separate(QString); //分离字符串方法，输入文法中存放产生式的字符串，将字符串分离后保存在Tokens中

};

#endif // PRODUCTION_H
