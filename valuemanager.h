#ifndef VALUEMANAGER_H
#define VALUEMANAGER_H

#include <QString>
#include "dbmanager.h"


class valueManager
{
public:
    valueManager();

    QStringList getPersonsArray(dbManager database);
    float getTotalIncome(dbManager database);
    float getTotalExpense(dbManager database);
};

#endif // VALUEMANAGER_H
