#include "valuemanager.h"

valueManager::valueManager()
{

}

QStringList valueManager::generatePersonsArray(dbManager database)
{
    QStringList l1;
    int i1 = database.incomesTableModel->rowCount();
    for (int i = 0; i < i1; i++)
    {
        l1.append(database.incomesTableModel->record(i).value("person").toString());
    }
    return l1;
}

float valueManager::generateTotalIncome(dbManager database)
{
    float toReturn = 0;
    for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
    {
        toReturn += database.incomesTableModel->record(i).value("value").toFloat();
    }
    return toReturn;
}

float valueManager::generateTotalExpense(dbManager database)
{
    float toReturn = 0;
    for (int i = 0; i < database.expensesTableModel->rowCount(); i++)
    {
        toReturn += database.expensesTableModel->record(i).value("value").toFloat();
    }
    return toReturn;
}
