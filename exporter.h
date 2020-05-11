#ifndef EXPORTER_H
#define EXPORTER_H
#include "dbmanager.h"
#include "valuemanager.h"


class Exporter
{
public:
    virtual void exportToTxt(QString filename) = 0;
    virtual void exportToExcel(QString filename) = 0;
    void printSuccess(QString filename);
    void printFailure(QString err);
    void printFailure(QSqlError err);
};

class SummaryExporter : public Exporter {
    dbManager dbm;
    valueManager vm;
public:
    void exportToTxt(QString filename) override;
    void exportToExcel(QString filename) override;
    SummaryExporter(dbManager dbMgr, valueManager vMgr);
};

class TableExporter : public Exporter {
    QString tablename;
    dbManager dbm;

public:
    void exportToTxt(QString filename) override;
    void exportToExcel(QString filename) override;
    TableExporter(QString table, dbManager database);
};

class BadTableNameException : QException {
public:
    void raise() const override { throw *this; };
    BadTableNameException *clone() const override { return new BadTableNameException(*this); };
};

#endif // EXPORTER_H
