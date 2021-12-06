#include "testdialog.h"
#include "qcustomplot.h"
#include "ui_testdialog.h"
#include "window.h"
#include <QDebug>
#include <QThread>
#include <dvb/dvbbackenddevice.h>
#include <dvb/dvbconfig.h>
#include <dvb/dvbdevice.h>
#include <dvb/dvbliveview.h>
#include <dvb/dvbscan.h>
#include <dvb/dvbtransponder.h>
#include <iostream>
#include <stdio.h>

static bool stopped_analize = false;

bool is_group;

class ScanThread : public QThread {
public:
  ScanThread(DvbScan *scan) { this->scan = scan; }

private:
  DvbScan *scan;
  void run() { scan->start(); }
};

TestDialog::TestDialog(QWidget *parent, DvbManager *manager)
    : QDialog(parent), ui(new Ui::TestDialog) {
  ui->setupUi(this);
  this->manager = manager;
  this->num_graphs = -1;

  this->max_found_snr = -1;
  this->min_found_snr = 32768;

  this->max_found_sig = 0.00452931;
  this->min_found_sig = 0.00385977;

  // oooops

  FILE *f = fopen("/home/sergey/dvb_drive.csv", "r");
  QTableWidget *table = ui->tablePoints;

  int r = 0;
  while (!feof(f)) {
    char buf[200];
    memset(buf, 0, sizeof(buf));
    fgets(buf, 200, f);
    QString b = QString::fromLocal8Bit(buf);
    QStringList lst = b.split(";");
    if (lst.size() < 6)
      continue;
    QString dateStr = lst[0];
    QString lat = lst[1];
    QString lon = lst[2];
    QString lock = lst[3];
    QString sig = lst[4];
    QString snr = lst[5];
    sig = sig.replace(",", ".");
    snr = snr.replace(",", ".");
    double Snr = snr.toDouble();
    double Sig = sig.toDouble();

    Sig = 1 / fabs(Sig);
    if (max_found_sig < Sig)
      max_found_sig = Sig;
    if (min_found_sig > Sig)
      min_found_sig = Sig;

    if (max_found_snr < Snr)
      max_found_snr = Snr;
    if (Snr != -1 && min_found_snr > Snr)
      min_found_snr = Snr;

    table->insertRow(r);
    dateStr = dateStr.replace("\"", "");
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(dateStr));
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(lat));
    table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(lon));
    table->setItem(table->rowCount() - 1, 3, new QTableWidgetItem(lock));
    table->setItem(table->rowCount() - 1, 4, new QTableWidgetItem(sig));
    table->setItem(table->rowCount() - 1, 5, new QTableWidgetItem(snr));

    r++;
  }
  fclose(f);

  qDebug() << "min_found_snr = " << min_found_snr << "\n";
  qDebug() << "max_found_snr = " << max_found_snr << "\n";

  qDebug() << "min_found_sig = " << min_found_sig << "\n";
  qDebug() << "max_found_sig = " << max_found_sig << "\n";

  min_found_snr = 15.5;
  max_found_snr = 39.4;

  min_found_sig = 0.00385977;
  max_found_sig = 0.0118816;

  // OpenGL Widget

  QGridLayout *layout = new QGridLayout(ui->widgetView);
  layout->addWidget(new Window(ui->widgetView));
}

TestDialog::~TestDialog() { delete ui; }

void TestDialog::on_pushButton_clicked() {
  //
  DvbDevice *device = manager->getDeviceConfigs().at(1).device;

  const DvbDeviceConfig &it = manager->getDeviceConfigs().at(1);

  foreach (const DvbConfig &config, it.configs) {
    if (config->name == "Terrestrial (T2)") {
      device = it.device;
      device->acquire(config.constData());
      qDebug() << config->name;
    }
  }

  if (device == NULL) {
    qDebug() << "No device!";
  }

  qDebug() << device->getDeviceId();
  DvbBackendDevice::Scale s;
  QString src = "Terrestrial (T2)";

  /*
      for (int f = 70000000; f < 110000000;f+=1000000) {

      DvbT2Transponder *t = new DvbT2Transponder();
      memset(t, 0, sizeof(DvbT2Transponder));
      t->bandwidth = DvbT2Transponder::Bandwidth1_7MHz;
      t->frequency = f;//522000000;
      t->modulation = DvbT2Transponder::ModulationAuto;
      t->fecRateHigh = DvbT2Transponder::FecAuto;
      t->fecRateLow = DvbT2Transponder::FecNone;
      t->guardInterval = DvbT2Transponder::GuardIntervalAuto;
      t->modulation = DvbT2Transponder::ModulationAuto;
      t->transmissionMode=DvbT2Transponder::TransmissionModeAuto;

      //qDebug() << t->toString();

      DvbTransponder transponder(DvbTransponderBase::DvbT2);

      DvbTransponder tt = transponder.fromString(t->toString());

      delete t;

      //qDebug() << tt.toString();


      device->tune(tt);
      float sig = 1/device->getSignal(s);
      float snr = device->getSnr(s);
      float ff = f / 1000000;
      std::cout << ff << ";" << sig << ";" << snr << "\n";
      fflush(stdout);

      }


      return;
  */

  DvbT2Transponder *t = new DvbT2Transponder();
  memset(t, 0, sizeof(DvbT2Transponder));
  t->bandwidth = DvbT2Transponder::Bandwidth8MHz;
  t->frequency = 522000000;
  t->modulation = DvbT2Transponder::Qam256; // DvbT2Transponder::ModulationAuto;
  t->fecRateHigh = DvbT2Transponder::FecAuto;
  t->fecRateLow = DvbT2Transponder::FecNone;
  t->guardInterval = DvbT2Transponder::GuardIntervalAuto;
  // t->modulation = DvbT2Transponder::ModulationAuto;
  t->transmissionMode = DvbT2Transponder::TransmissionModeAuto;

  // qDebug() << t->toString();

  DvbTransponder transponder(DvbTransponderBase::DvbT2);

  DvbTransponder tt = transponder.fromString(t->toString());

  device->tune(tt);

  FILE *f = fopen("/home/sergey/dvb_test.csv", "w");

  char gps_norm[] = "0;0;";

  while (true) {

    device->getProps(tt);

    DvbT2Transponder *dvb = tt.as<DvbT2Transponder>();

    switch (dvb->modulation) {
    case 0:
      qDebug() << "QPSK\n";
      break;
    case 1:
      qDebug() << "Qam16\n";
      break;
    case 2:
      qDebug() << "Qam64\n";
      break;
    case 3:
      qDebug() << "Qam256\n";
      break;
    default:
      qDebug() << "unknown mod\n";
    };

    float sig = device->getSignal(s);
    float snr = device->getSnr(s);

    qDebug() << "sig = " << sig << " snr = " << snr << "\n";

    char gp[100];
    memset(gp, 0, sizeof(gp));
    FILE *gps = fopen("/home/sergey/gps.txt", "r");
    if (gps)
      fgets(gp, 100, gps);
    fclose(gps);
    if (strlen(gp) > 0) {
      strcpy(gps_norm, gp);
    }

    qDebug() << "GPS: " << gps_norm << "\n";

    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss.zzz");

    fprintf(f, "\"%s\";%s%d;%f;%f\n", formattedTime.toLocal8Bit().data(),
            gps_norm, device->isTuned(), -device->getSignal(s),
            device->getSnr(s));
    fflush(f);

    QThread::currentThread()->msleep(777);
  }
}

/*
 * Spectrum analizer
 */

void TestDialog::on_pushButtonAnalize_clicked() {

  double maxs = this->max_found_sig; // ONLY FOR ASTROMETA CARD!
  double mins = this->min_found_sig;

  const int mille = 1000000;

  double max_us = maxs;

  do {

    int f1 = (int)(ui->lineEditFrom->text().toFloat() * mille);
    int f2 = (int)(ui->lineEditTo->text().toFloat() * mille);
    int step =
        ui->comboBoxStep->itemText(ui->comboBoxStep->currentIndex()).toInt() *
        mille;

    /* prepare the graph */

    QCustomPlot *customPlot = ui->plot;

    // if we need to prepare a fresh graph
    if (num_graphs == -1 || !ui->checkBoxAppend->isChecked()) {
      customPlot->clearGraphs();
      customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
      // set a more compact font size for bottom and left axis tick labels:
      customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
      customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
      // set axis labels:
      customPlot->xAxis->setLabel("Freq");
      customPlot->yAxis->setLabel("Signal strength from DVB card");
      // make top and right axes visible but without ticks and labels:
      customPlot->xAxis2->setVisible(true);
      customPlot->yAxis2->setVisible(true);
      customPlot->xAxis2->setTicks(false);
      customPlot->yAxis2->setTicks(false);
      customPlot->xAxis2->setTickLabels(false);
      customPlot->yAxis2->setTickLabels(false);
      // set axis ranges to show all data:
      customPlot->xAxis->setRange(f1 / mille, f2 / mille);
      customPlot->yAxis->setRange(mins, max_us * 1.2);
      // show legend with slightly transparent background brush:
      customPlot->legend->setVisible(true);
      customPlot->legend->setBrush(QColor(255, 255, 255, 150));

      ui->plot->axisRect()->setupFullAxesBox(true);
      ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
      num_graphs = 0;
      max_us = 0;

    } else {
      num_graphs++;
    }

    /* prepare the card and scan params */
    stopped_analize = false;

    if (manager->getDeviceConfigs().size() == 0)
      return;

    DvbDevice *device = manager->getDeviceConfigs().at(1).device;

    const DvbDeviceConfig &it = manager->getDeviceConfigs().at(1);

    DvbConfig cfg;
    foreach (const DvbConfig &config, it.configs) {
      if (config->name == "Terrestrial (T2)") {
        device = it.device;
        device->acquire(config.constData());
        cfg = config;
        qDebug() << config->name;
        ui->labelCard->setText(device->getFrontendName());
        break;
      }
    }

    if (device == NULL) {
      qDebug() << "No device!";
      return;
    }

    qDebug() << "dev: " << device->getDeviceId();
    DvbBackendDevice::Scale s;
    QString src = "Terrestrial (T2)";

    int countPoints = (f2 - f1 + 1) / step + 1;

    if (num_graphs > 4) { // max graph count
      num_graphs = 0;
      customPlot->clearGraphs();
    }

    // set different colors based on graph number from example
    int gi = num_graphs;
    customPlot->addGraph();
    QColor color(20 + 200 / 4.0 * gi, 70 * (1.6 - gi / 4.0), 150, 150);
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(QPen(color.lighter(200)));
    customPlot->graph()->setBrush(QBrush(color));
    customPlot->graph()->setName(
        QDateTime::currentDateTime().toString("hh:mm:ss"));

    QVector<QCPGraphData> signalData(countPoints);

    int i = 0;

    DvbT2Transponder *transpRaw = new DvbT2Transponder();
    memset(transpRaw, 0, sizeof(DvbT2Transponder));
    transpRaw->bandwidth = DvbT2Transponder::Bandwidth1_7MHz;
    transpRaw->frequency = 500000000;
    transpRaw->modulation =
        DvbT2Transponder::Qam256; // DvbT2Transponder::ModulationAuto;
    transpRaw->fecRateHigh = DvbT2Transponder::FecAuto;
    transpRaw->fecRateLow = DvbT2Transponder::FecNone;
    transpRaw->guardInterval = DvbT2Transponder::GuardIntervalAuto;
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionModeAuto;

    DvbTransponder transponder(DvbTransponderBase::DvbT2);
    DvbTransponder transpRepr = transponder.fromString(transpRaw->toString());

    QApplication::processEvents();

    for (int f = f1; f <= f2; f += step) {

      if (stopped_analize)
        break;
      float fMhz = f / mille;

      ui->lineEditCurrent->setText(QString::number((int)fMhz));

      transpRaw->frequency = f;
      transpRepr = transponder.fromString(transpRaw->toString());
      device->tune(transpRepr);

      /* obtain the signal data */
      float sig = device->getSignal(s);
      float sig2 = 1 / sig;

      if (sig2 > maxs)
        maxs = sig2;

      if (sig == 100)
        sig2 = 0;

      // float snr = device->getSnr(s);
      // double sig2 = sigs.at(i);

      signalData[i].key = fMhz;
      signalData[i].value = sig2;

      /* do plotting */
      customPlot->graph()->data()->set(signalData);
      ui->plot->replot();

      // qDebug() << fMhz << ";" << sig << ";" << sig2 << ";" << snr << "\n";

      QApplication::processEvents();
      // QThread::currentThread()->msleep(100);

      i++;
    }

    qDebug() << "done!";

    device->release();

    // update scale
    customPlot->yAxis->setRange(mins, max_us * 1.2);
    ui->plot->axisRect()->setupFullAxesBox(true);
    customPlot->yAxis->rescale();
    ui->plot->replot();

    QApplication::processEvents();

  } while (ui->checkBoxNonStop->isChecked());
}

void TestDialog::on_pushButtonStopAnalize_clicked() { stopped_analize = true; }

void TestDialog::on_checkBoxGroupNearest_stateChanged(int arg1) {
  is_group = !is_group;
  ui->widgetView->repaint();
}

void TestDialog::on_pushButtonGISStartScan_clicked() {
  //
}
