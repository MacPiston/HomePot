#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "dbmanager.h"
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

private slots:
    void on_newPushButton_clicked();
    void on_loadPushButton_clicked();

    void on_actionOpen_triggered();

    void on_personListSelectorWidget_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;
    dbManager database;
};
#endif // MAINWINDOW_H
