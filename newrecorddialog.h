#ifndef INCOMESNEWINCOMEDIALOG_H
#define INCOMESNEWINCOMEDIALOG_H

#include <QDialog>

namespace Ui {
class newRecordDialog;
}

class newRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newRecordDialog(QWidget *parent = nullptr);
    ~newRecordDialog();

private slots:
    void on_cancelPushButton_clicked();

private:
    Ui::newRecordDialog *ui;
};

#endif // INCOMESNEWINCOMEDIALOG_H
