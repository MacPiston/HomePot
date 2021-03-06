#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "dbmanager.h"
#include "valuemanager.h"
#include "chartbuilder.h"
#include <QMainWindow>
#include <QtSql>
#include <QPieSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadData();
    void switchEnabledElements(bool state);
    void loadPersonData();
    void loadIncomesTabData();
    void loadExpensesTabData();
    void loadSummaryData();
protected:
    Ui::MainWindow *ui;
    dbManager database;
    valueManager vManager;
    ChartBuilder cBuilder;
private slots:
    void on_newPushButton_clicked();
    void on_loadPushButton_clicked();
    void on_actionOpen_triggered();
    //void on_personListSelectorWidget_currentRowChanged(int currentRow);
    void on_incomesSubmitButton_clicked();
    void on_incomesDeleteIncomeButton_clicked();
    void on_incomesNewIncomeButton_clicked();
    void on_expensesNewIncomeButton_clicked();
    void on_expensesDeleteIncomeButton_clicked();
    void on_expensesSubmitButton_clicked();
    void on_MainWindow_destroyed();
    void on_actionExit_triggered();
    void on_exportIncomesPushButton_clicked();
    void on_exportExpensesPushButton_clicked();
    void on_exportSummaryPushButton_clicked();
    void on_personListSelectorWidget_itemClicked(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
