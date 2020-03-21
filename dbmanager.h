#include <QtSql>
#include <QTableView>
#ifndef DBMANAGER_H
#define DBMANAGER_H

class dbManager
{
private:
    QSqlDatabase sqlDatabase;
public:
    dbManager();
    void createNewDatabase(QString filename);
    void openExistingDatabase(QString filename);
    bool isOpen();
    QSqlTableModel *getTableModel(const QString &tableName);
};

#endif // DBMANAGER_H
