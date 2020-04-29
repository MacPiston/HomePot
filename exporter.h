#ifndef EXPORTER_H
#define EXPORTER_H
#include "dbmanager.h"


class Exporter
{
public:
    virtual void exportToTxt(dbManager database, QString filename) = 0;
    virtual void exportToExcel(dbManager database, QString filename) = 0;
};

class SummaryExporter : public Exporter {
public:
    void exportToTxt(dbManager database, QString filename) override;
    void exportToExcel(dbManager database, QString filename) override;
};

class TableExporter : public Exporter {
    QString tableName;
public:
    void exportToTxt(dbManager database, QString filename) override;
    void exportToExcel(dbManager database, QString filename) override;
    TableExporter(QString table);
};

class BadTableNameException : QException {
public:
    void raise() const override { throw *this; };
    BadTableNameException *clone() const override { return new BadTableNameException(*this); };
};

#endif // EXPORTER_H
