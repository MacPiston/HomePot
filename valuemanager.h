#ifndef VALUEMANAGER_H
#define VALUEMANAGER_H

#include <QString>
#include "dbmanager.h"


class valueManager
{
public:
    valueManager();

    QStringList generatePersonsArray(dbManager database);
    float generateTotalIncome(dbManager database);
    float generateTotalExpense(dbManager database);
};

#endif // VALUEMANAGER_H
