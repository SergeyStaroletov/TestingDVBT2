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

static bool stoppedAnalize = false;

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

  // opengl widget
  // QRect sz = ui->widgetView->contentsRect();
  // ui->widgetView=new Window(this->window());

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

  // DvbScan* scan = new DvbScan(device, src, tt, false);

  // ScanThread * th = new ScanThread(scan);
  // th->start();

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

  int mille = 1000000;
  int f1 = (int)(ui->lineEditFrom->text().toFloat() * mille);
  int f2 = (int)(ui->lineEditTo->text().toFloat() * mille);
  int step =
      ui->comboBoxStep->itemText(ui->comboBoxStep->currentIndex()).toInt() *
      mille;

  FILE *f = fopen("/home/sergey/sig.txt", "r");

  QTableWidget *table = ui->tablePoints;

  std::vector<int> freqs;
  std::vector<double> sigs;

  double maxs = -1;
  double mins = 32768;

  int r = 0;
  while (!feof(f)) {
    char buf[200];
    memset(buf, 0, sizeof(buf));
    fgets(buf, 200, f);
    QString b = QString::fromLocal8Bit(buf);
    QStringList lst = b.split(";");
    if (lst.size() < 3)
      continue;
    QString freq = lst[0];
    QString sig = lst[1];
    sig = sig.replace(",", ".");
    int fr = freq.toInt();
    double sigg = sig.toDouble();
    if (sigg > maxs)
      maxs = sigg;
    if (sigg < mins)
      mins = sigg;
    freqs.push_back(fr);
    sigs.push_back(sigg);
    r++;
  }
  fclose(f);

  /* prepare the graph */

  QCustomPlot *customPlot = ui->plot;
  customPlot->clearGraphs();
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

  double now = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
  srand(8); // set the random seed, so we always get the same random data

  // configure left axis text labels:
  // QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  // textTicker->addTick(10, "a bit\nlow");
  // textTicker->addTick(50, "quite\nhigh");
  // customPlot->yAxis->setTicker(textTicker);
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
  customPlot->yAxis->setRange(mins, maxs * 1.2);
  // show legend with slightly transparent background brush:
  customPlot->legend->setVisible(true);
  customPlot->legend->setBrush(QColor(255, 255, 255, 150));

  ui->plot->axisRect()->setupFullAxesBox(true);
  ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  /* prepare the card and scan params */

  stoppedAnalize = false;

  if (manager->getDeviceConfigs().size() == 0)
    return;

  DvbDevice *device = manager->getDeviceConfigs().at(1).device;
  device->getDeviceId();
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

  int gi = 0;
  customPlot->addGraph();
  QColor color(20 + 200 / 4.0 * gi, 70 * (1.6 - gi / 4.0), 150, 150);
  customPlot->graph()->setLineStyle(QCPGraph::lsLine);
  customPlot->graph()->setPen(QPen(color.lighter(200)));
  customPlot->graph()->setBrush(QBrush(color));

  QVector<QCPGraphData> signalData(countPoints);

  int i = 0;

  /*DvbT2Transponder *transp = new DvbT2Transponder();
  memset(transp, 0, sizeof(DvbT2Transponder));
  transp->bandwidth = DvbT2Transponder::Bandwidth8MHz;
  transp->frequency = 522000000;
  transp->modulation = DvbT2Transponder::ModulationAuto;
  transp->fecRateHigh = DvbT2Transponder::FecAuto;
  transp->fecRateLow = DvbT2Transponder::FecNone;
  transp->guardInterval = DvbT2Transponder::GuardIntervalAuto;
  transp->modulation = DvbT2Transponder::ModulationAuto;
  transp->transmissionMode = DvbT2Transponder::TransmissionModeAuto;
    */

  DvbT2Transponder *t = new DvbT2Transponder();
  memset(t, 0, sizeof(DvbT2Transponder));
  t->bandwidth = DvbT2Transponder::Bandwidth1_7MHz;
  t->frequency = 500000000;
  t->modulation = DvbT2Transponder::Qam256; // DvbT2Transponder::ModulationAuto;
                                            // //DvbT2Transponder::Qam256;
  t->fecRateHigh = DvbT2Transponder::FecAuto;
  t->fecRateLow = DvbT2Transponder::FecNone;
  t->guardInterval = DvbT2Transponder::GuardIntervalAuto;
  t->transmissionMode = DvbT2Transponder::TransmissionModeAuto;

  DvbTransponder transponder(DvbTransponderBase::DvbT2);
  DvbTransponder transpRepr = transponder.fromString(t->toString());
  // delete transp;

  QApplication::processEvents();

  device->tune(transpRepr);

  QThread::currentThread()->msleep(2000);

  // for (int f : freqs) {

  for (int f = f1; f <= f2; f += step) {

    if (stoppedAnalize)
      break;
    float fMhz = f / mille;

    ui->lineEditCurrent->setText(QString::number((int)fMhz));

    t->frequency = f;
    transpRepr = transponder.fromString(t->toString());
    device->tune(transpRepr);

    /* obtain the signal data */
    float sig = device->getSignal(s);
    float sig2 = 1 / sig;
    float snr = device->getSnr(s);

    // double sig2 = sigs.at(i);

    signalData[i].key = fMhz;
    signalData[i].value = sig2;

    /* do plotting */
    customPlot->graph()->data()->set(signalData);
    ui->plot->replot();

    qDebug() << fMhz << ";" << sig << ";" << sig2 << ";" << snr << "\n";

    QApplication::processEvents();
    // QThread::currentThread()->msleep(100);

    i++;
  }

  qDebug() << "done!";

  device->release();

  /* create multiple graphs:
  for (int gi=0; gi<5; ++gi)
  {
    customPlot->addGraph();
    QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(QPen(color.lighter(200)));
    customPlot->graph()->setBrush(QBrush(color));
    // generate random walk data:
    QVector<QCPGraphData> timeData(250);
    for (int i=0; i<250; ++i)
    {
      timeData[i].key = now + 24*3600*i;
      if (i == 0)
        timeData[i].value = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
      else
        timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-gi)) +
  (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
      customPlot->graph()->data()->set(timeData);
      ui->plot->replot();

      QThread::currentThread()->msleep(100);
      QApplication::processEvents();
    }
  }
  */
}

void TestDialog::on_pushButtonStopAnalize_clicked() { stoppedAnalize = true; }

void TestDialog::on_checkBoxGroupNearest_stateChanged(int arg1) {
  is_group = !is_group;
  ui->widgetView->repaint();
}
