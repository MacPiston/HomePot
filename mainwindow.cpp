#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartbuilder.h"
#include <QPieSeries>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ChartBuilder cBuilder;

    ui->chartViewOverall->setChart(cBuilder.buildChart("none", false));
    ui->chartViewOverall->setRenderHint(QPainter::Antialiasing);

}

MainWindow::~MainWindow()
{
    delete ui;
}

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

}

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
    ui->actionSave_As->setEnabled(state);
}

void MainWindow::reloadPersonData()
{

}


void MainWindow::on_newPushButton_clicked()
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    database.createNewDatabase(dbFilename);
    if (database.isOpen())
    {
        ui->savePushButton->setEnabled(true);
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
        ui->savePushButton->setEnabled(true);
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
    reloadPersonData(); //TODO
}
