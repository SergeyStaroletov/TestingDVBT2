#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <dvb/dvbmanager.h>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent, DvbManager *manager);
    ~TestDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestDialog *ui;
    DvbManager *manager;

};

#endif // TESTDIALOG_H
