#ifndef FOLLOW_H
#define FOLLOW_H

#include <QList>
#include "First.h"

/**
 * @brief The Follow class
 */
class Follow
{
public:
    Follow();
    QList<QString> getFollow();
    bool setFollow(Follow);
    bool setFollow(First);
    bool setFollow(QString);
private:
    QList<QString> follow;
};

#endif // FOLLOW_H
