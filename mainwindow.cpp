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
    QString *personArray = database.getDataArray(database.personsTableModel, "person");
    float *floatArray = database.getFloatArray(database.expensesTableModel, "value");
    //person selector

    for (int i = 0;i < database.personsTableModel->rowCount(); i++)
    {
        ui->personListSelectorWidget->addItem(personArray[i]);
    }
    ui->personListSelectorWidget->sortItems(Qt::SortOrder::AscendingOrder);
    ui->personListSelectorWidget->setSortingEnabled(true);
    ui->personListSelectorWidget->setCurrentRow(0);

    //total expense
    float sum = 0;
    if (database.expensesTableModel->rowCount() != 0)
    {
        for (int i = 0; i < database.expensesTableModel->rowCount(); i++)
        {
         sum += floatArray[i];
        }
    }
    ui->totalExpenseValue->setText(QString::number(sum));

    //total income
    sum = 0;
    if (database.incomesTableModel->rowCount() != 0)
    {
        floatArray = database.getFloatArray(database.incomesTableModel, "value");
        for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
        {
            sum += floatArray[i];
        }
    }
    ui->totalIncomeValue->setText(QString::number(sum));

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
    ui->incomesTableView->setModel(database.incomesTableModel);
    ui->incomesTableView->resizeRowsToContents();

    float totalValue = 0;
    float *floatArray = database.getFloatArray(database.incomesTableModel, "value");
    for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
    {
        totalValue += floatArray[i];
    }
    ui->incomesYearlyValueLabel->setText(QString::number(totalValue));
}

//RESPONDING TO EVENTS
void MainWindow::on_newPushButton_clicked() // new database creation
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    database.createNewDatabase(dbFilename);
    if (database.isOpen())
    {
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
}

void MainWindow::on_incomesDeleteIncomeButton_clicked() // deleting selected rows on INCOMES view
{
    QItemSelectionModel *selectedRows = ui->incomesTableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        QModelIndexList rows = selectedRows->selectedIndexes();
        database.incomesTableModel->removeRows(rows.first().row(), rows.count());
        database.incomesTableModel->database().transaction();
        if (database.incomesTableModel->submitAll())
        {
            database.incomesTableModel->database().commit();
        } else
        {
            database.incomesTableModel->database().rollback();
        }
    }
}

void MainWindow::on_incomesNewIncomeButton_clicked() // adding new income
{
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
}
