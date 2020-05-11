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
    float getPersonExpense(dbManager database, QString person);
    float getPersonIncome(dbManager database, QString person);
};

#endif // VALUEMANAGER_H
