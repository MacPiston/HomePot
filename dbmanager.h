#include <QtSql>
#include <QTableView>
#ifndef DBMANAGER_H
#define DBMANAGER_H

class dbManager
{
private:
    QSqlDatabase sqlDatabase;
    std::shared_ptr<QSqlTableModel> getTableModel(const QString &tableName);
public:
    dbManager();

    std::shared_ptr<QSqlTableModel> expensesTableModel;
    std::shared_ptr<QSqlTableModel> incomesTableModel;
    QStringList getDataArray(std::shared_ptr<QSqlTableModel> model, QString value);
    QSqlDatabase getDatabase();

    void createNewDatabase(QString filename);
    void openExistingDatabase(QString filename);
    void closeDatabase();
    bool isOpen();
};

#endif // DBMANAGER_H
