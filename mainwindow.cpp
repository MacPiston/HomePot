#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPieSeries>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();
    series->append("Jane", 1);
    series->append("Joe", 2);
    series->append("Andy", 3);
    series->append("Barbara", 4);
    series->append("Axel", 5);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();

    ui->chartview->setChart(chart);
    ui->chartview->setRenderHint(QPainter::Antialiasing);
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
