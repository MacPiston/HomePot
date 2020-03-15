#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
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

    void createDatabase(QString filename);
    void loadData();
    void switchEnabledElements(bool state);

private slots:
    void on_newPushButton_clicked();
    void on_loadPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
};
#endif // MAINWINDOW_H
