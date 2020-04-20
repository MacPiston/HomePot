#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartbuilder.h"
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

    //MONTH COMBOBOX
    ui->monthComboBox->addItems(months);
    ui->incomesMonthComboBox->addItems(months);
    ui->expensesMonthComboBox->addItems(months);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//------DATA LOADING-------
void MainWindow::loadData()
{
    //chart
    ui->chartViewOverall->setChart(cBuilder.buildEIChart(false, vManager, database));
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
    float totalExpense = vManager.generateTotalExpense(database);
    ui->totalExpenseValue->setText(QString::number(totalExpense));

    //total income
    float totalIncome = vManager.generateTotalIncome(database);
    ui->totalIncomeValue->setText(QString::number(totalIncome));

    //status
    float diff = totalIncome - totalExpense;
    if ((diff > -1000) and (diff < 0))
    {
        ui->statusValue->setText("Warning: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color : orange; color : black;}");
    } else if (diff < -1000)
    {
        ui->statusValue->setText("ALERT: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color : red; color : black;}");
    } else {
        ui->statusValue->setText("Fine: " + QString::number(totalIncome - totalExpense));
        ui->statusValue->setStyleSheet("QLabel { background-color : green; color : black;}");
    }
    loadPersonData(); //CAUSES CRASH WHEN SUBMITTING EXPENSES
}

void MainWindow::loadPersonData()
{
    ui->personListSelectorWidget->clear();
    ui->personListSelectorWidget->setLayoutMode(QListView::SinglePass);
    ui->personListSelectorWidget->addItems(vManager.generatePersonsArray(database));
    ui->personListSelectorWidget->sortItems(Qt::SortOrder::AscendingOrder);
    ui->personListSelectorWidget->setSortingEnabled(true);
}

void MainWindow::loadIncomesTabData()
{
    database.incomesTableModel->setTable("incomes");
    database.incomesTableModel->sort(0, Qt::SortOrder::AscendingOrder);
    ui->incomesTableView->setModel(database.incomesTableModel);

    ui->incomesYearlyValueLabel->setText(QString::number(vManager.generateTotalIncome(database)));
    ui->incomesTableView->update();
}

void MainWindow::loadExpensesTabData()
{
    database.expensesTableModel->setTable("expenses");
    database.expensesTableModel->sort(0, Qt::SortOrder::AscendingOrder);
    ui->expensesTableView->setModel(database.expensesTableModel);

    ui->expensesYearlyValueLabel->setText(QString::number(vManager.generateTotalExpense(database)));
    ui->expensesTableView->update();
}

//------RESPONDING TO EVENTS------

//--SUMMARY BUTTONS
void MainWindow::on_newPushButton_clicked() // new database creation
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    database.createNewDatabase(dbFilename);
    if (database.isOpen())
    {
        loadData();
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
    }
}

void MainWindow::on_loadPushButton_clicked() // loading existing database
{
    QString dbFilename = QFileDialog::getOpenFileName(this, tr("Open Budget Database"), tr(".../"), tr("*.sqlite"));
    database.openExistingDatabase(dbFilename);
    if (database.isOpen())
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
    float pExpense = 0;
    float pIncome = 0;

    QSqlQuery pExpensesQuery;
    pExpensesQuery.prepare("SELECT value FROM expenses WHERE person = ?");
    pExpensesQuery.bindValue(0, person);
    pExpensesQuery.exec();
    while (pExpensesQuery.next())
    {
        pExpense += pExpensesQuery.value(0).toFloat();
    }
    ui->personLastExpense->setText(QString::number(pExpense));

    QSqlQuery pIncomesQuery;
    pIncomesQuery.prepare("SELECT value FROM incomes WHERE person = ?");
    pIncomesQuery.bindValue(0, person);
    pIncomesQuery.exec();
    while (pIncomesQuery.next())
    {
        pIncome += pIncomesQuery.value(0).toFloat();
    }
    ui->personLastIncome->setText(QString::number(pIncome));
}

//--INCOMES BUTTONS
void MainWindow::on_incomesSubmitButton_clicked() // submitting changes to the database on INCOMES view
{
    database.incomesTableModel->database().transaction();
    if (database.incomesTableModel->submitAll())
    {
        database.incomesTableModel->database().commit();
    } else
    {
        database.incomesTableModel->database().rollback();
    }
    loadIncomesTabData();
    loadData();
}

void MainWindow::on_incomesDeleteIncomeButton_clicked() // deleting selected rows on INCOMES view
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

//--EXPENSES BUTTONS
void MainWindow::on_expensesNewIncomeButton_clicked() //adding new expense
{
    database.expensesTableModel->insertRows(0, 1);
}

void MainWindow::on_expensesDeleteIncomeButton_clicked()
{
    QItemSelectionModel *selectedRows = ui->expensesTableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        QModelIndexList rows = selectedRows->selectedIndexes();
        database.expensesTableModel->removeRows(rows.first().row(), rows.count());
    }
}

void MainWindow::on_expensesSubmitButton_clicked()
{
    database.expensesTableModel->database().transaction();
    if (database.expensesTableModel->submitAll())
    {
        database.expensesTableModel->database().commit();
    } else
    {
        database.expensesTableModel->database().rollback();
    }
    loadExpensesTabData();
    loadData();
}

//--------OTHERS--------
void MainWindow::switchEnabledElements(bool state)
{
    ui->personListSelectorWidget->setEnabled(state);
    ui->totalIncome->setEnabled(state);
    ui->totalIncomeValue->setEnabled(state);
    ui->totalExpense->setEnabled(state);
    ui->totalExpenseValue->setEnabled(state);
    ui->monthlyRadioButton->setEnabled(state);
    ui->monthComboBox->setEnabled(state);
    ui->yearlyRadioButton->setEnabled(state);
    ui->tabWidget->setTabEnabled(1, state);
    ui->tabWidget->setTabEnabled(2, state);
    ui->incomeTab->setEnabled(state);
    ui->expensesTab->setEnabled(state);
    ui->statusValue->setEnabled(state);
    ui->status->setEnabled(state);
}

void MainWindow::on_MainWindow_destroyed()
{
    database.closeDatabase();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
