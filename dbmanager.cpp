#include "dbmanager.h"
#include <QtSql>

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
                "PRIMARY KEY(\"name\"))");
        query.exec("CREATE TABLE \"incomes\" "
                "(\"name\"	TEXT NOT NULL,"
                "\"value\"	REAL NOT NULL,"
                "\"category\"	TEXT NOT NULL,"
                "\"date\"	TEXT NOT NULL,"
                "PRIMARY KEY(\"name\"))");
        query.exec("CREATE TABLE \"people\" "
                "(\"name\"	TEXT)");
    }
}

void dbManager::openExistingDatabase(QString filename)
{
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName(filename);
    if (!sqlDatabase.open())
    {

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
