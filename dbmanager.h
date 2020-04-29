#include <QtSql>
#include <QTableView>
#ifndef DBMANAGER_H
#define DBMANAGER_H

class dbManager
{
private:
    QSqlDatabase sqlDatabase;
    QSqlTableModel *getTableModel(const QString &tableName);
public:
    dbManager();

    QSqlTableModel *expensesTableModel;
    QSqlTableModel *incomesTableModel;
    QStringList getDataArray(QSqlTableModel *model, QString value);
    QSqlDatabase getDatabase();

    void createNewDatabase(QString filename);
    void openExistingDatabase(QString filename);
    void closeDatabase();
    bool isOpen();
};

#endif // DBMANAGER_H
