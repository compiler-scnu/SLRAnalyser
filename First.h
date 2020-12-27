#ifndef FIRST_H
#define FIRST_H

#include <QList>

/**
 * @brief The First class
 */
class First
{
public:
    First();
    QList<QString> getFirst();
    bool setFirst(First);
    bool setFirst(QString);

private:
    QList<QString> first;
};

#endif // FIRST_H
