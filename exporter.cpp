#include "exporter.h"
#include "xlsxdocument.h"
#include "fstream"
#include <QMessageBox>
#include <QDebug>

void SummaryExporter::exportToTxt(dbManager database, QString filename) {

}

void SummaryExporter::exportToExcel(dbManager database, QString filename) {
    QXlsx::Document xlsx;

}

void TableExporter::exportToTxt(dbManager database, QString tablename, QString filename) {
    if (tablename != "incomes" and tablename != "expenses") {
        throw BadTableNameException();
    } else {
        QSqlQuery query(database.getDatabase());
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
        }
    }
}

void TableExporter::exportToExcel(dbManager database, QString tablename, QString filename) {
    if (tablename != "incomes" and tablename != "expenses") {
        throw BadTableNameException();
    } else {
        QXlsx::Document xlsx;
        QSqlQuery query(database.getDatabase());
        query.prepare("SELECT * FROM " + tablename);
        if (query.exec()) {
            //TODO
        }
    }
}
