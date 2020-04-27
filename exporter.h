#ifndef EXPORTER_H
#define EXPORTER_H


class exporter
{
public:
    virtual void exportToTxt() = 0;
    virtual void exportToExcel() = 0;
};

class summaryExporter : public exporter {
public:
    void exportToTxt() override;
    void exportToExcel() override;;
};

class incomesExporter : public exporter {
public:
    void exportToTxt() override;
    void exportToExcel() override;
};

class expensesExporter : public exporter {
public:
    void exportToTxt() override;
    void exportToExcel() override;
};

#endif // EXPORTER_H
