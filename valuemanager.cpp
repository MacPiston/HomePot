#include "valuemanager.h"

valueManager::valueManager()
{

}

QStringList valueManager::getPersonsArray(dbManager database)
{
    QStringList l1;

    for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
    {
        l1.append(database.incomesTableModel->record(i).value("person").toString());
    }

    for (int i = 0; i < database.expensesTableModel->rowCount(); i++)
    {
        l1.append(database.expensesTableModel->record(i).value("person").toString());
    }
    l1.removeDuplicates();
    return l1;
}

float valueManager::getTotalIncome(dbManager database)
{
    float toReturn = 0;
    for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
    {
        toReturn += database.incomesTableModel->record(i).value("value").toFloat();
    }
    return toReturn;
}

float valueManager::getTotalExpense(dbManager database)
{
    float toReturn = 0;
    for (int i = 0; i < database.expensesTableModel->rowCount(); i++)
    {
        toReturn += database.expensesTableModel->record(i).value("value").toFloat();
    }
    return toReturn;
}

float valueManager::getPersonExpense(dbManager database, QString person) {
    float toReturn = 0;
    QSqlQuery pExpenseQuery(database.getDatabase());
    pExpenseQuery.prepare("SELECT value FROM expenses WHERE person = ?");
    pExpenseQuery.bindValue(0, person);
    pExpenseQuery.exec();
    while (pExpenseQuery.next()) {
        toReturn += pExpenseQuery.value(0).toFloat();
    }
    return toReturn;

}

float valueManager::getPersonIncome(dbManager database, QString person) {
    float toReturn = 0;
    QSqlQuery pIncomeQuery(database.getDatabase());
    pIncomeQuery.prepare("SELECT value FROM incomes WHERE person = ?");
    pIncomeQuery.bindValue(0, person);
    pIncomeQuery.exec();
    while (pIncomeQuery.next()) {
        toReturn += pIncomeQuery.value(0).toFloat();
    }
    return toReturn;
}
