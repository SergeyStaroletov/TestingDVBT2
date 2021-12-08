#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <dvb/dvbbackenddevice.h>
#include <dvb/dvbmanager.h>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog {
  Q_OBJECT

public:
  explicit TestDialog(QWidget *parent, DvbManager *manager);
  ~TestDialog();

private slots:
  void on_pushButton_clicked();

  void on_pushButtonAnalize_clicked();

  void on_pushButtonStopAnalize_clicked();

  void on_checkBoxGroupNearest_stateChanged(int arg1);

  void on_pushButtonGISStartScan_clicked();

  void on_buttonStartLocking_clicked();

  void on_buttonStopLocking_clicked();

  void on_buttonAddTransponder_clicked();

  void on_buttonRemoveTransponder_clicked();

  void on_checkBoxFast_stateChanged(int arg1);

private:
  Ui::TestDialog *ui;
  DvbManager *manager;
  int num_graphs;
  double max_found_sig;
  double min_found_sig;
  double max_found_snr;
  double min_found_snr;
  bool is_fast_lock;
};

class DvbGradProgress2 : public QLabel {
public:
  explicit DvbGradProgress2(QWidget *parent);
  ~DvbGradProgress2();

  void setValue(float value_, DvbBackendDevice::Scale scale, float _min,
                float _max);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  float value, max, min;
};

#endif // TESTDIALOG_H
