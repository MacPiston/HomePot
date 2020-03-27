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
    QSqlTableModel *personsTableModel;

    void createNewDatabase(QString filename);
    void openExistingDatabase(QString filename);
    bool isOpen();

    QString *getDataArray(QSqlTableModel *model, QString value);
    float *getFloatArray(QSqlTableModel *model, QString value);
};

#endif // DBMANAGER_H
