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

    //CHART
    ChartBuilder cBuilder;
    ui->chartViewOverall->setChart(cBuilder.buildChart("none", false));
    ui->chartViewOverall->setRenderHint(QPainter::Antialiasing);

    //TABS
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);

    //MONTH COMBOBOX
    ui->monthComboBox->addItems(months);
    ui->incomesMonthComboBox->addItems(months);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// DATA LOADING
void MainWindow::loadData()
{
    //setup

    //person selector
    QStringList persons = vManager.generatePersonsArray(database);
    ui->personListSelectorWidget->addItems(persons);
    ui->personListSelectorWidget->sortItems(Qt::SortOrder::AscendingOrder);
    ui->personListSelectorWidget->setSortingEnabled(true);
    ui->personListSelectorWidget->setCurrentRow(0);


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
    //incomes tab
    loadIncomesTabData();

    //expenses tab

}

void MainWindow::loadPersonData()
{
    //TODO
}

void MainWindow::loadIncomesTabData()
{
    database.incomesTableModel->sort(0, Qt::SortOrder::AscendingOrder);
    ui->incomesTableView->setModel(database.incomesTableModel);
    ui->incomesTableView->resizeRowsToContents();

    ui->incomesYearlyValueLabel->setText(QString::number(vManager.generateTotalIncome(database)));
    ui->incomesTableView->update();
}

//RESPONDING TO EVENTS
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
    ui->personListSelectorWidget->setCurrentRow(currentRow);
    loadPersonData(); //TODO
}

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

//OTHERS
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
