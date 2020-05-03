#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartbuilder.h"
#include "exporter.h"
#include <QDebug>
#include <QPieSeries>
#include <QFileDialog>

//CONSTRUCTOR & DESTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //UI
    ui->setupUi(this);

    //TABS
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);

    //BUTTONS
    ui->exportSummaryPushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//------DATA LOADING-------
void MainWindow::loadData()
{
    //chart
    ui->chartViewOverall->setChart(cBuilder.buildEIChart(vManager, database));
    ui->chartViewOverall->setRenderHint(QPainter::Antialiasing);

    //summary
    loadSummaryData();

    //incomes tab
    loadIncomesTabData();

    //expenses tab
    loadExpensesTabData();
}

void MainWindow::loadSummaryData()
{
    //total expense
    float totalExpense = vManager.getTotalExpense(database);
    ui->totalExpenseValue->setText(QString::number(totalExpense));

    //total income
    float totalIncome = vManager.getTotalIncome(database);
    ui->totalIncomeValue->setText(QString::number(totalIncome));

    //status
    float diff = totalIncome - totalExpense;
    if ((diff > -1000) and (diff < 0))
    {
        ui->statusValue->setText("Warning: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color: orange; color: black; border-radius: 9px}");
    } else if (diff < -1000)
    {
        ui->statusValue->setText("ALERT: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color: red; color: black; border-radius: 9px}");
    } else {
        ui->statusValue->setText("Good: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color: green; color: black; border-radius: 9px}");
    }
    loadPersonData(); //znowu się tu wypierdala..... edit: póki co nie
}

void MainWindow::loadPersonData()
{
    ui->personListSelectorWidget->clear();
    ui->personListSelectorWidget->setLayoutMode(QListView::SinglePass);
    ui->personListSelectorWidget->addItems(vManager.getPersonsArray(database));
    ui->personListSelectorWidget->sortItems(Qt::SortOrder::AscendingOrder);
    ui->personListSelectorWidget->setSortingEnabled(true);
}

void MainWindow::loadIncomesTabData()
{
    database.incomesTableModel->setTable("incomes");
    database.incomesTableModel->sort(0, Qt::SortOrder::AscendingOrder);

    ui->incomesTableView->setModel(database.incomesTableModel.get());
    ui->incomesTotalValueLabel->setText(QString::number(vManager.getTotalIncome(database)));
    //ui->incomesTableView->update(); // powoduje crasha i chyba niepotrzebne
}

void MainWindow::loadExpensesTabData()
{
    database.expensesTableModel->setTable("expenses");
    database.expensesTableModel->sort(0, Qt::SortOrder::AscendingOrder);

    ui->expensesTableView->setModel(database.expensesTableModel.get());
    ui->expensesTotalValueLabel->setText(QString::number(vManager.getTotalExpense(database)));
    //ui->expensesTableView->update(); // powoduje crasha i chyba nie potrzebne
}

//------RESPONDING TO EVENTS------

//--SUMMARY BUTTONS
void MainWindow::on_newPushButton_clicked() // new database creation
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    //qDebug() << dbFilename;
    database.createNewDatabase(dbFilename);
    if (dbFilename != "")
    {
        loadData();
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
    }
}

void MainWindow::on_loadPushButton_clicked() // loading existing database
{
    QString dbFilename = QFileDialog::getOpenFileName(this, tr("Open Budget Database"), tr(".../"), tr("*.sqlite"));
    //qDebug() << dbFilename;
    database.openExistingDatabase(dbFilename);
    if (dbFilename != "")
    {   
        loadData();
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
    }
}

void MainWindow::on_actionOpen_triggered() // loading existing database by top menu
{
    on_loadPushButton_clicked();
}

void MainWindow::on_personListSelectorWidget_currentRowChanged(int currentRow) // response to change in person selector
{
    QString person = ui->personListSelectorWidget->item(currentRow)->text();
    float personExpense = 0;
    float personIncome = 0;
    float totalExpense = vManager.getTotalExpense(database);
    float totalIncome = vManager.getTotalIncome(database);

    QSqlQuery pExpensesQuery(database.getDatabase());
    pExpensesQuery.prepare("SELECT value FROM expenses WHERE person = ?");
    pExpensesQuery.bindValue(0, person);
    pExpensesQuery.exec();
    while (pExpensesQuery.next())
    {
        personExpense += pExpensesQuery.value(0).toFloat();
    }
    ui->personLastExpense->setText(QString::number(personExpense));
    ui->personExpensePercent->setText(QString::number(personExpense * 100 / totalExpense) + "%");

    QSqlQuery pIncomesQuery(database.getDatabase());
    pIncomesQuery.prepare("SELECT value FROM incomes WHERE person = ?");
    pIncomesQuery.bindValue(0, person);
    pIncomesQuery.exec();
    while (pIncomesQuery.next())
    {
        personIncome += pIncomesQuery.value(0).toFloat();
    }
    ui->personLastIncome->setText(QString::number(personIncome));
    ui->personIncomePercent->setText(QString::number(personIncome * 100 / totalIncome) + "%");
}

//--INCOMES BUTTONS
void MainWindow::on_incomesSubmitButton_clicked() // saving changes made earlier
{
    database.incomesTableModel->database().transaction();
    if (database.incomesTableModel->submitAll())
    {
        database.incomesTableModel->database().commit();
    } else
    {
        database.incomesTableModel->database().rollback();
    }
    loadData();
}

void MainWindow::on_incomesDeleteIncomeButton_clicked() // deleting selected incomes
{
    QItemSelectionModel *selectedRows = ui->incomesTableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        QModelIndexList rows = selectedRows->selectedIndexes();
        database.incomesTableModel->removeRows(rows.first().row(), rows.count());
    }
}

void MainWindow::on_incomesNewIncomeButton_clicked() // adding new income
{
    database.incomesTableModel->insertRows(0, 1);

}

void MainWindow::on_exportIncomesPushButton_clicked() //exorting incomes data to file
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export to .txt file"), tr("incomesExport"), tr("Text file (*.txt);;Excel datasheet (*.xlsx)"));

    if (!filename.isEmpty() and filename.endsWith(".txt")) {
        TableExporter e1("incomes", database);
        try {
            e1.exportToTxt(filename);
        } catch (BadTableNameException &ex) {

        }
    } else if (!filename.isEmpty() and filename.endsWith(".xlsx")) {
        TableExporter e2("incomes", database);
        try {
            e2.exportToExcel(filename);
        } catch (BadTableNameException &ex) {

        }
    } else {
        QMessageBox errorBox;
        errorBox.setText("Couldn't export file:");
        errorBox.setDetailedText("Wrong file extension selected");
        errorBox.setDefaultButton(QMessageBox::Ok);
        errorBox.exec();
    }

}

//--EXPENSES BUTTONS
void MainWindow::on_expensesNewIncomeButton_clicked() //adding new expense
{
    database.expensesTableModel->insertRows(0, 1);
}

void MainWindow::on_expensesDeleteIncomeButton_clicked() //deleting selected expenses
{
    QItemSelectionModel *selectedRows = ui->expensesTableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        QModelIndexList rows = selectedRows->selectedIndexes();
        database.expensesTableModel->removeRows(rows.first().row(), rows.count());
    }
}

void MainWindow::on_expensesSubmitButton_clicked() //saving changes made earlier
{
    database.expensesTableModel->database().transaction();
    if (database.expensesTableModel->submitAll())
    {
        database.expensesTableModel->database().commit();
    } else
    {
        database.expensesTableModel->database().rollback();
    }
    loadData();
}

void MainWindow::on_exportExpensesPushButton_clicked() //exporting expenses data to file
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export to .txt file"), tr("expensesExport"), tr("Text file (*.txt);;Excel datasheet (*.xlsx)"));

    if (!filename.isEmpty() and filename.endsWith(".txt")) {
        TableExporter e1("expenses", database);
        try {
            e1.exportToTxt(filename);
        } catch (BadTableNameException &ex) {

        }
    } else if (!filename.isEmpty() and filename.endsWith(".xlsx")) {
        TableExporter e2("expenses", database);
        try {
            e2.exportToExcel(filename);
        } catch (BadTableNameException &ex) {

        }
    } else {
        QMessageBox errorBox;
        errorBox.setText("Couldn't export file:");
        errorBox.setDetailedText("Wrong file extension selected");
        errorBox.setDefaultButton(QMessageBox::Ok);
        errorBox.exec();
    }
}

//--------OTHERS--------
void MainWindow::switchEnabledElements(bool state)
{
    ui->personListSelectorWidget->setEnabled(state);
    ui->totalIncome->setEnabled(state);
    ui->totalIncomeValue->setEnabled(state);
    ui->totalExpense->setEnabled(state);
    ui->totalExpenseValue->setEnabled(state);
    ui->tabWidget->setTabEnabled(1, state);
    ui->tabWidget->setTabEnabled(2, state);
    ui->status->setEnabled(state);
    ui->exportSummaryPushButton->setEnabled(state);
}

void MainWindow::on_MainWindow_destroyed()
{
    database.closeDatabase();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


