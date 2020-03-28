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
    ui->setupUi(this);
    ChartBuilder cBuilder;

    ui->chartViewOverall->setChart(cBuilder.buildChart("none", false));
    ui->chartViewOverall->setRenderHint(QPainter::Antialiasing);

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);

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
    for (int i = 0; i < database.expensesTableModel->rowCount(); i++)
    {
        sum += floatArray[i];
    }
    ui->totalExpenseValue->setText(QString::number(sum));

    //total income
    sum = 0;
    floatArray = database.getFloatArray(database.incomesTableModel, "value");
    for (int i = 0; i < database.incomesTableModel->rowCount(); i++)
    {
        sum += floatArray[i];
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
void MainWindow::on_newPushButton_clicked()
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    database.createNewDatabase(dbFilename);
    if (database.isOpen())
    {
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
    }
}

void MainWindow::on_loadPushButton_clicked()
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

void MainWindow::on_actionOpen_triggered()
{
    on_loadPushButton_clicked();
}

void MainWindow::on_personListSelectorWidget_currentRowChanged(int currentRow)
{
    ui->personListSelectorWidget->setCurrentRow(currentRow);
    loadPersonData(); //TODO
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
    ui->actionSave->setEnabled(state);
    ui->tabWidget->setTabEnabled(1, state);
    ui->tabWidget->setTabEnabled(2, state);
    ui->incomeTab->setEnabled(state);
    ui->expensesTab->setEnabled(state);
}
