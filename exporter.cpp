#include "exporter.h"
#include "xlsxdocument.h"
#include "xlsxchart.h"
#include "fstream"
#include "valuemanager.h"
#include "iterator"
#include <QMessageBox>
#include <QDebug>

void SummaryExporter::exportToTxt(QString filename) {
        QStringList persons = vm.getPersonsArray(dbm);
        float totalExpense = vm.getTotalExpense(dbm);
        float totalIncome = vm.getTotalIncome(dbm);

        std::ofstream outputFile;
        outputFile.open(filename.toStdString(), std::ofstream::trunc);
        if (outputFile.is_open()) {
        outputFile << "---- Summary ----\n";
        outputFile << "Total expense: " << totalExpense << "\n";
        outputFile << "Total income: " << totalIncome << "\n";

        outputFile << "---- Persons ---- (total expense/income)\n";
        for (auto p : persons) {
            outputFile << p.toStdString() << " : " << vm.getPersonExpense(dbm, p) << " " << vm.getPersonIncome(dbm, p) << "\n";
        }
        outputFile << "Exported on: " << QDateTime::currentDateTime().toString("hh:mm dd.MM.yy").toStdString() << "\n";
        outputFile.close();
        printSuccess(filename);
        } else {
            printFailure(filename);
        }
}

void SummaryExporter::exportToExcel(QString filename) {
    QXlsx::Document xlsx;
    QXlsx::Format mergeFormat;
    QXlsx::Format boldFormat;
    QStringList persons = vm.getPersonsArray(dbm);
    mergeFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    mergeFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    boldFormat.setFontBold(true);

    xlsx.write("A1", "Summary:", boldFormat);
    xlsx.mergeCells("A1:C1");

    xlsx.write("A2", "Total expense", boldFormat);
    xlsx.write("B2", vm.getTotalExpense(dbm));

    xlsx.write("A3", "Total income", boldFormat);
    xlsx.write("B3", vm.getTotalIncome(dbm));

    xlsx.write("A4", "Persons", boldFormat);
    xlsx.mergeCells("A4:C4", mergeFormat);
    xlsx.write("A5", "Name", boldFormat);
    xlsx.write("B5", "Total Expense", boldFormat);
    xlsx.write("C5", "Total Income", boldFormat);

    int row = 6;
    for (int p = 0; p < persons.size(); p++, row++) {
        xlsx.write(row, 1, persons[p]);
        xlsx.write(row, 2, vm.getPersonExpense(dbm, persons[p]));
        xlsx.write(row, 3, vm.getPersonIncome(dbm, persons[p]));
    }
    xlsx.autosizeColumnWidth();

    xlsx.write(row, 1, "Exported on", boldFormat);
    xlsx.write(row, 2, QDateTime::currentDateTime().toString("hh:mm dd.MM.yy"));

    QXlsx::Chart *pieChart = xlsx.insertChart(3, 4, QSize(300, 300));
    pieChart->setChartType(QXlsx::Chart::CT_PieChart);
    pieChart->addSeries(QXlsx::CellRange("B2:B3"));
    pieChart->setChartTitle("Summary");

    if (xlsx.saveAs(filename)) {
        printSuccess(filename);
    } else {
        printFailure("Failed to save .xlsx file");
    }
}

SummaryExporter::SummaryExporter(dbManager dbMgr, valueManager vMgr) {
    this->vm = vMgr;
    this->dbm = dbMgr;
}

TableExporter::TableExporter(QString table, dbManager database) {
    this->tablename = table;
    this->dbm = database;
}

void TableExporter::exportToTxt(QString filename) {
    if (tablename != "incomes" and tablename != "expenses") {
        throw BadTableNameException();
    } else {
        QSqlQuery query(dbm.getDatabase());
        std::map<std::string, float> personSum;

        query.prepare("SELECT * FROM " + tablename);
        if (query.exec()) {
            std::ofstream outputFile;
            outputFile.open(filename.toStdString(), std::ofstream::trunc);

            outputFile << "-------- Records --------\n";
            while (query.next()) {
                std::string person = query.value("person").toString().toStdString();
                double value = query.value("value").toDouble();
                outputFile << query.value("name").toString().toStdString() << " ";
                outputFile << value << " ";
                outputFile << query.value("category").toString().toStdString() << " ";
                outputFile << query.value("date").toString().toStdString() << " ";
                outputFile << person << "\n";

                if (personSum.find(person) == personSum.end()) {
                    personSum.insert(std::pair<std::string, double>(person, value));
                } else {
                    personSum[person] += value;
                }
            }

            outputFile << "-------- Summary --------\n";
            for (std::map<std::string, float>::iterator it = personSum.begin(), end = personSum.end(); it != end; it = personSum.upper_bound(it->first)) {
                outputFile << it->first << " " << it->second << "\n";
            }
            outputFile << "Exported on: " << QDateTime::currentDateTime().toString("hh:mm dd.MM.yy").toStdString() << "\n";

            outputFile.close();
            printSuccess(filename);
        } else {
            printFailure(query.lastError());
        }
    }
}

void TableExporter::exportToExcel(QString filename) {
    if (tablename != "incomes" and tablename != "expenses") {
        throw BadTableNameException();
    } else {
        QSqlQuery query(dbm.getDatabase());
        query.prepare("SELECT * FROM " + tablename);
        if (query.exec()) {
            QXlsx::Document xlsx;
            QXlsx::Format boldFormat;
            boldFormat.setFontBold(true);

            xlsx.write(1, 1, "Name", boldFormat);
            xlsx.write(1, 2, "Value", boldFormat);
            xlsx.write(1, 3, "Category", boldFormat);
            xlsx.write(1, 4, "Date", boldFormat);
            xlsx.write(1, 5, "Person", boldFormat);
            int rowCounter = 2;

            while (query.next()) {
                xlsx.write(rowCounter, 1, query.value("name").toString());
                xlsx.write(rowCounter, 2, query.value("value").toDouble());
                xlsx.write(rowCounter, 3, query.value("category").toString());
                xlsx.write(rowCounter, 4, query.value("date").toString());
                xlsx.write(rowCounter, 5, query.value("person").toString());
                rowCounter++;
            }

            xlsx.currentSheet()->sheetName() = "Incomes";
            xlsx.autosizeColumnWidth();

            if (xlsx.saveAs(filename)) {
                printSuccess(filename);
            } else {
                printFailure("Failed to save .xlsx file");
            }
        } else {
            printFailure(query.lastError());
        }
    }
}

void Exporter::printFailure(QSqlError err) {
    qDebug() << err;
    QMessageBox errorBox;
    errorBox.setText("Failed to export file:");
    errorBox.setDetailedText(err.text());
    errorBox.setDefaultButton(QMessageBox::Ok);
    errorBox.exec();
}

void Exporter::printSuccess(QString filename) {
    QMessageBox infoBox;
    infoBox.setText("Exported to: " + filename);
    infoBox.setDefaultButton(QMessageBox::Ok);
    infoBox.exec();
}

void Exporter::printFailure(QString err) {
    qDebug() << err;
    QMessageBox errorBox;
    errorBox.setText("Failed to export file:");
    errorBox.setDetailedText(err);
    errorBox.setDefaultButton(QMessageBox::Ok);
    errorBox.exec();
}
