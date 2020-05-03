#include "exporter.h"
#include "xlsxdocument.h"
#include "fstream"
#include <QMessageBox>
#include <QDebug>

void SummaryExporter::exportToTxt(QString filename) {

}

void SummaryExporter::exportToExcel(QString filename) {
    QXlsx::Document xlsx;

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
        query.prepare("SELECT * FROM " + tablename);
        if (query.exec()) {
            std::ofstream outputFile;
            outputFile.open(filename.toStdString(), std::ofstream::trunc);

            while (query.next()) {
                outputFile << query.value("name").toString().toStdString() << " ";
                outputFile << query.value("value").toDouble() << " ";
                outputFile << query.value("category").toString().toStdString() << " ";
                outputFile << query.value("date").toString().toStdString() << " ";
                outputFile << query.value("person").toString().toStdString() << "\n";
            }
            outputFile.close();

            QMessageBox infoBox;
            infoBox.setText("Exported to: " + filename);
            infoBox.setDefaultButton(QMessageBox::Ok);
            infoBox.exec();
        } else {
            qDebug() << query.lastError();
            QMessageBox errorBox;
            errorBox.setText("Failed to export file:");
            errorBox.setDetailedText(query.lastError().text());
            errorBox.setDefaultButton(QMessageBox::Ok);
            errorBox.exec();
        }
    }
}

void TableExporter::exportToExcel(QString filename) {
    if (tablename != "incomes" and tablename != "expenses") {
        throw BadTableNameException();
    } else {
        QXlsx::Document xlsx;
        QSqlQuery query(dbm.getDatabase());
        query.prepare("SELECT * FROM " + tablename);
        if (query.exec()) {
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
            xlsx.saveAs(filename);
        }
    }
}
