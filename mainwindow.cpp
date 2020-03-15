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

void MainWindow::createDatabase(QString filename)
{
    QSqlDatabase newDb = QSqlDatabase::addDatabase("QSQLITE");
    newDb.setDatabaseName(filename);
    if (newDb.open())
    {
      database = newDb;
    }
    else
    {
       //TODO - ERROR
    }
}

void MainWindow::loadData()
{

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
}


void MainWindow::on_newPushButton_clicked()
{
    QString dbFilename = QFileDialog::getSaveFileName(this, tr("Create Budget Database"), tr("newBudget.sqlite"), tr("*.sqlite"));
    createDatabase(dbFilename);
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
    createDatabase(dbFilename);
    if (database.isOpen())
    {
        ui->savePushButton->setEnabled(true);
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
    }
}
