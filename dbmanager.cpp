#include "dbmanager.h"
#include <QtSql>
#include <iostream>

dbManager::dbManager()
{

}

void dbManager::createNewDatabase(QString filename)
{
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
        query.exec("CREATE TABLE \"persons\" "
                "(\"person\"	TEXT)");
    }
}

void dbManager::openExistingDatabase(QString filename)
{
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName(filename);
    if (sqlDatabase.open())
    {
        personsTableModel = getTableModel("persons");
        expensesTableModel = getTableModel("expenses");
        incomesTableModel = getTableModel("incomes");
    } else {

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

QString *dbManager::getDataArray(QSqlTableModel *model, QString value)
{
    int rows = model->rowCount();
    QString *toReturn = new QString[rows];
    for (int i = 0; i < rows; i++)
    {
        toReturn[i] = model->record(i).value(value).toString();
    }
    return toReturn;
}

float *dbManager::getFloatArray(QSqlTableModel *model, QString value)
{
    int rows = model->rowCount();
    float *toReturn = new float[rows];
    for (int i = 0; i < rows; i++) {
        toReturn[i] = model->record(i).value(value).toFloat();
    }
    return toReturn;
}

