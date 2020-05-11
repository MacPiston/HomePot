#include "exporter.h"
#include "xlsxdocument.h"
#include "fstream"
#include "valuemanager.h"
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

        outputFile << "---- Persons ---- (expense income)\n";
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
            for (auto element = personSum.begin(), end = personSum.end(); element != end; element = personSum.upper_bound(element->first)) {
                outputFile << element->first << " " << element->second << "\n";
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
            int rowCounter = 1;

            while (query.next()) {
                xlsx.write(rowCounter, 1, query.value("name").toString());
                xlsx.write(rowCounter, 2, query.value("value").toDouble());
                xlsx.write(rowCounter, 3, query.value("category").toString());
                xlsx.write(rowCounter, 4, query.value("date").toString());
                xlsx.write(rowCounter, 5, query.value("person").toString());
                rowCounter++;
            }

            xlsx.currentSheet()->sheetName() = "Incomes";

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
