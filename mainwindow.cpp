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
    database.createNewDatabase(filename);
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
    ui->actionSave->setEnabled(state);
    ui->actionSave_As->setEnabled(state);
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
        ui->savePushButton->setEnabled(true);
        ui->budgetNameLabel->setText(dbFilename);
        switchEnabledElements(true);
        QSqlTableModel *model = database.getTableModel("expenses");
        QString newItem = model->record(0).value("name").toString();
        ui->personListSelectorWidget->addItem(newItem);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    on_loadPushButton_clicked();
}
