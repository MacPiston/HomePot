#include "dbmanager.h"
#include <QtSql>
#include <iostream>

dbManager::dbManager()
{

}

void dbManager::createNewDatabase(QString filename)
{
    if (sqlDatabase.isOpen()) sqlDatabase.close();
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName(filename);
    if (!sqlDatabase.open())
    {
        //TODO-ERROR
    } else {
        QSqlQuery query;
        query.exec("CREATE TABLE \"expenses\""
                "(\"name\"	TEXT NOT NULL,"
                "\"value\"	REAL NOT NULL,"
                "\"category\"	TEXT NOT NULL,"
                "\"date\"	TEXT NOT NULL,"
                "\"person\"	TEXT NOT NULL,"
                "PRIMARY KEY(\"name\"))");
        query.exec("CREATE TABLE \"incomes\" "
                "(\"name\"	TEXT NOT NULL,"
                "\"value\"	REAL NOT NULL,"
                "\"category\"	TEXT NOT NULL,"
                "\"date\"	TEXT NOT NULL,"
                "\"person\"	TEXT NOT NULL,"
                "PRIMARY KEY(\"name\"))");
        expensesTableModel = getTableModel("expenses");
        incomesTableModel = getTableModel("incomes");
    }
}

void dbManager::openExistingDatabase(QString filename)
{
    if (sqlDatabase.isOpen()) sqlDatabase.close();
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName(filename);
    if (!sqlDatabase.open())
    {
        //TODO - ERROR
    } else {
        expensesTableModel = getTableModel("expenses");
        incomesTableModel = getTableModel("incomes");
    }
}

bool dbManager::isOpen()
{
    return sqlDatabase.isOpen();
}

QSqlTableModel *dbManager::getTableModel(const QString &tableName)
{
    QSqlTableModel *model = new QSqlTableModel(nullptr, sqlDatabase);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    return model;
}

QStringList dbManager::getDataArray(QSqlTableModel *model, QString value)
{
    int rows = model->rowCount();
    QStringList toReturn;
    for (int i = 0; i < rows; i++)
    {
        toReturn.append(model->record(i).value(value).toString());
    }
    return toReturn;
}


