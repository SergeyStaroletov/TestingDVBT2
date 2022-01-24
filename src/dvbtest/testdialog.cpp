#include "testdialog.h"
#include "glwidget.h"
#include "ui_testdialog.h"
#include "window.h"
#include <KLocalizedString>
#include <QDebug>
#include <QMutex>
#include <QThread>

#include "dvbtest/rtlfetcherthread.h"

#include "dvbtest/pidscan.h"

#include <iostream>
#include <stdio.h>

bool stopped_analize = false;

extern QMap<int, int> dvb_stat; // data on pid -> len
QMap<int, QVector<char>> dvb_char_data;

bool should_collect_data = false;
bool should_save_data = false;

bool is_group;

DvbGradProgress2::DvbGradProgress2(QWidget *parent)
    : QLabel(parent), value(0), max(100.), min(0.) {
  setAlignment(Qt::AlignCenter);
  setFrameShape(Box);
  setText("");
}

DvbGradProgress2::~DvbGradProgress2() {}

void DvbGradProgress2::setValue(float value_, DvbBackendDevice::Scale scale,
                                float max_, float min_) {
  QString text;
  value = value_;

  switch (scale) {
  case DvbBackendDevice::NotSupported: {
    text = '-';
    max = 100;
    min = 0;
    break;
  }
  case DvbBackendDevice::Percentage: {
    float value2 = (value_ - min_) / (max_ - min_) * 100;
    text = QString::number(value2, 'f', 0) + '%';
    max = max_;
    min = min_;
    break;
  }
  case DvbBackendDevice::Decibel: {
    text = QString::number(value, 'f', 2) + " dB";
    max = 40;
    min = 0;
    break;
  }
  case DvbBackendDevice::dBuV: {
    text =
        QString::number(value, 'f', 2) + " dB" + QString((QChar)0x00b5) + 'V';
    max = 80;
    min = 20;
    break;
  }
  }

  setText(i18n("%1", text));
  update();
}

void DvbGradProgress2::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  int border = frameWidth();
  QRect rect(border, border, width() - 2 * border, height() - 2 * border);
  QLinearGradient gradient(rect.topLeft(), rect.topRight());
  gradient.setColorAt(0, Qt::red);
  gradient.setColorAt(1, Qt::green);
  if (value < min)
    value = min;
  if (value > max)
    value = max;
  rect.setWidth((rect.width() * (value - min)) / (max - min));
  painter.fillRect(rect, gradient);

  QLabel::paintEvent(event);
}

class ScanThread : public QThread {
public:
  ScanThread(DvbScan *scan) { this->scan = scan; }

private:
  DvbScan *scan;
  void run() { scan->start(); }
};

QString TestDialog::DetectDVBCard(DvbDevice **device) {
  *device = nullptr;
  QString card;

  if (manager->getDeviceConfigs().size() == 0)
    return "";

  *device = manager->getDeviceConfigs().at(1).device;
  const DvbDeviceConfig &it = manager->getDeviceConfigs().at(1);
  DvbConfig cfg;
  foreach (const DvbConfig &config, it.configs) {
    if (config->name == "Terrestrial (T2)") {
      *device = it.device;
      if (!*device)
        return "";
      (*device)->acquire(config.constData());
      cfg = config;
      card = (*device)->getFrontendName();
      break;
    }
  }
  return card;
}

bool TestDialog::TuneToTranspId(int id_transp) {

  QTableWidget *table = ui->tableTranspondersList;

  if (id_transp < 0) {
    QMessageBox::warning(this, "Scan", "Please select a transponder", 0, 0);
    return false;
  }

  stopped_analize = false;

  QString card = this->DetectDVBCard(&device);
  ui->labelCard->setText(card);

  if (!device) {
    qDebug() << "No device!";
    return false;
  }

  // tune to a transponder
  transpRaw = new DvbT2Transponder();
  memset(transpRaw, 0, sizeof(DvbT2Transponder));
  if (is_fast_lock) {
    transpRaw->bandwidth = DvbT2Transponder::Bandwidth1_7MHz;
  } else {
    transpRaw->bandwidth = DvbT2Transponder::Bandwidth8MHz;
  };
  QString mod = table->item(id_transp, 3)->text().toLower();
  transpRaw->modulation = DvbT2Transponder::ModulationAuto;
  if (mod == "qam256")
    transpRaw->modulation = DvbT2Transponder::Qam256;
  if (mod == "qam16")
    transpRaw->modulation = DvbT2Transponder::Qam16;
  if (mod == "qam64")
    transpRaw->modulation = DvbT2Transponder::Qam64;
  if (mod == "Qpsk")
    transpRaw->modulation = DvbT2Transponder::Qpsk;

  QString fec = table->item(id_transp, 1)->text().toLower();
  transpRaw->fecRateHigh = DvbT2Transponder::FecAuto;
  if (fec == "1/2")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec1_2;
  if (fec == "1/3")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec1_3;
  if (fec == "1/4")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec1_4;
  if (fec == "2/3")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec2_3;
  if (fec == "2/5")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec2_5;
  if (fec == "3/4")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec3_4;
  if (fec == "3/5")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec3_5;
  if (fec == "4/5")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec4_5;
  if (fec == "5/6")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec5_6;
  if (fec == "6/7")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec6_7;
  if (fec == "7/8")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec7_8;
  if (fec == "8/9")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec8_9;
  if (fec == "9/10")
    transpRaw->fecRateHigh = DvbT2Transponder::Fec9_10;

  transpRaw->fecRateLow = DvbT2Transponder::FecNone;
  transpRaw->guardInterval = DvbT2Transponder::GuardIntervalAuto;

  QString tm = table->item(id_transp, 2)->text().toLower();
  transpRaw->transmissionMode = DvbT2Transponder::TransmissionModeAuto;
  if (tm == "1k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode1k;
  if (tm == "2k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode2k;
  if (tm == "4k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode4k;
  if (tm == "8k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode8k;
  if (tm == "16k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode16k;
  if (tm == "32k")
    transpRaw->transmissionMode = DvbT2Transponder::TransmissionMode32k;

  transpRaw->frequency = table->item(id_transp, 0)->text().toDouble() * 1000000;
  transpRepr = transponder.fromString(transpRaw->toString());
  device->tune(transpRepr);

  return true;
}

void TestDialog::LoadGPSPoints() {
  const QString fileName = QDir::home().absolutePath() + "/dvb_drive.csv";
  if (QFile::exists(fileName)) {
    FILE *f = fopen(fileName.toLocal8Bit(), "r");
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
  }
}

TestDialog::TestDialog(QWidget *parent, DvbManager *manager)
    : QDialog(parent), ui(new Ui::TestDialog),
      transponder(DvbTransponderBase::DvbT2) {
  ui->setupUi(this);
  this->is_fast_lock = false;

  this->manager = manager;
  this->num_graphs = -1;

  this->iq_buffer = new unsigned char[BUF_RTL_SIZE];
  this->iq_buffer_len = 0;

  this->max_found_snr = -1;
  this->min_found_snr = 32768;

  // todo: check the name of dvb card in order to use these consts
  this->max_found_sig = 0.00452931;
  this->min_found_sig = 0.00385977;

  LoadGPSPoints();

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
  Window *w = new Window(ui->widgetView);
  this->glWidget = w->getGlWidget();
  layout->addWidget(w);

  // reference streams
  QDir directory(QDir::home().absolutePath() + "/dvbtest");
  QStringList tests =
      directory.entryList(QStringList() << "*.txt", QDir::Files);
  ui->comboBoxConstellRefStream->clear();
  ui->comboBoxConstellRefStream->addItem("");
  for (QString reftest : tests) {
    ui->comboBoxConstellRefStream->addItem(reftest);
  }

  // connect to a widget in order to populate it with data from ref streams
  connect(this, &TestDialog::newRawDataSignal, glWidget, &GLWidget::setData);
}

TestDialog::~TestDialog() { delete ui; }

void TestDialog::on_pushButton_clicked() {
  // the code to remove and rewrite to the gps tab

  DvbDevice *device;
  this->DetectDVBCard(&device);

  if (device == NULL) {
    qDebug() << "No device!";
  }

  qDebug() << device->getDeviceId();
  DvbBackendDevice::Scale s;
  QString src = "Terrestrial (T2)";

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
    int step;

    QString step_str =
        ui->comboBoxStep->itemText(ui->comboBoxStep->currentIndex());
    if (step_str != "0.5")
      step = step_str.toInt() * mille;
    else
      step = 0.5 * mille;

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

    DvbDevice *device;
    QString card = this->DetectDVBCard(&device);

    const DvbDeviceConfig &it = manager->getDeviceConfigs().at(1);

    ui->labelCard->setText(card);

    if (!device) {
      qDebug() << "No device!";
      return;
    }

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

      if (sig2 == 0 || sig2 == -1)
        sig2 = mins;

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

void TestDialog::on_buttonStartLocking_clicked() {

  QTableWidget *table = ui->tableTranspondersList;
  int row = table->currentRow();

  this->TuneToTranspId(row);

  int timer_no_signal = 0;
  const int pause_ms = 100;

  while (!stopped_analize) {
    // obtain values
    float sig = device->getSignal(scale);

    QString sig_str = QString::number(sig, 'g', 4);
    switch (scale) {
    case DvbBackendDevice::NotSupported:
      sig_str += " pts";
      break;
    case DvbBackendDevice::Percentage:
      sig_str += " %%";
      break;
    case DvbBackendDevice::Decibel:
      sig_str += " dB";
      break;
    case DvbBackendDevice::dBuV:
      sig_str += " dBuV";
    }
    ui->labelSigLev->setText(sig_str);

    float sig2 = 1 / sig;
    if (sig == 100)
      sig2 = 0;
    float snr = device->getSnr(scale);

    QString snr_str = QString::number(snr, 'g', 4);
    switch (scale) {
    case DvbBackendDevice::NotSupported:
      snr_str += " pts";
      break;
    case DvbBackendDevice::Percentage:
      snr_str += " %%";
      break;
    case DvbBackendDevice::Decibel:
      snr_str += " dB";
      break;
    case DvbBackendDevice::dBuV:
      snr_str += " dBuV";
    }
    ui->labelSnrLev->setText(snr_str);

    ui->labelSig->setValue(sig2, DvbBackendDevice::Percentage,
                           this->max_found_sig, this->min_found_sig);

    if (snr != -1) {
      ui->checkBoxLocked->setChecked(true);
      ui->labelSnr->setValue(snr, DvbBackendDevice::Percentage,
                             this->max_found_snr, this->min_found_snr);
      timer_no_signal = 0;

    } else {
      ui->checkBoxLocked->setChecked(false);
      ui->labelSnr->setValue(0, DvbBackendDevice::Percentage,
                             this->max_found_snr, 0);
      timer_no_signal += pause_ms;
    }

    // we have no signal more than 30 cycles
    if (timer_no_signal > pause_ms * 30) {
      // retune
      qDebug() << "retuning... \n";
      if (is_fast_lock) {
        transpRaw->bandwidth = DvbT2Transponder::Bandwidth1_7MHz;
      } else {
        transpRaw->bandwidth = DvbT2Transponder::Bandwidth8MHz;
      };
      transpRepr = transponder.fromString(transpRaw->toString());
      device->tune(transpRepr);
      timer_no_signal = 0;
    }

    QThread::currentThread()->msleep(pause_ms);
    QApplication::processEvents();
  }

  device->release();
}

void TestDialog::on_buttonStopLocking_clicked() { stopped_analize = true; }

void TestDialog::on_buttonAddTransponder_clicked() {

  QTableWidget *table = ui->tableTranspondersList;
  table->insertRow(table->rowCount());
}

void TestDialog::on_buttonRemoveTransponder_clicked() {
  QTableWidget *table = ui->tableTranspondersList;
  int r = table->currentRow();
  if (r >= 0)
    table->removeRow(r);
}

void TestDialog::on_checkBoxFast_stateChanged(int arg1) {
  is_fast_lock = ui->checkBoxFast->isChecked();
}

void TestDialog::on_buttonObtainData_clicked() {
  //
  QString freq = ui->comboBoxConstellFreq->currentText();
  int freqi = freq.toDouble() * 1000000;

  RTLFetcherThread *thread =
      new RTLFetcherThread(freqi, this->iq_buffer, &this->iq_buffer_len,
                           &stopped_analize, &this->sem_buffer);
  if (!thread->setup()) {
    return;
  }

  this->ui->buttonObtainData->setEnabled(false);

  connect(thread, &RTLFetcherThread::newRawDataSignal, glWidget,
          &GLWidget::setData);
  thread->start();

  return;
}

void TestDialog::on_tabWidget_currentChanged(int index) {
  // stop current work
  stopped_analize = true;
  // update transponders
  ui->comboBoxConstellFreq->clear();
  ui->comboBoxFreqPID->clear();
  QTableWidget *table = ui->tableTranspondersList;
  for (int r = 0; r < table->rowCount(); r++) {
    ui->comboBoxConstellFreq->addItem(table->item(r, 0)->text());
    ui->comboBoxFreqPID->addItem(table->item(r, 0)->text());
  }
}

void TestDialog::on_pushButtonStopRtl_clicked() {

  this->ui->buttonObtainData->setEnabled(true);
  stopped_analize = true;
}

void TestDialog::on_comboBoxConstellRefStream_activated(
    const QString &refname) {

  const int MAX_LEN = 256;
  // code to process a reference stream

  QString fname = QDir::home().absolutePath() + "/dvbtest/" + refname;

  FILE *f = fopen(fname.toLocal8Bit(), "r");
  if (!f)
    return;

  iq_data.clear();
  char buffer[MAX_LEN];
  int l = 0;
  bool hex = false;
  while (fgets(buffer, MAX_LEN - 1, f)) {
    l++;
    buffer[strcspn(buffer, "\n")] = 0;

    if (buffer[0] != '#' && buffer[0] != '%') {
      if (hex) {
        for (int i = 0; i < strlen(buffer); i += 2) {
          int b0 = buffer[i] - '0';
          if (buffer[i] >= 'A' && buffer[i] <= 'F')
            b0 = (buffer[i] - 'A') + 10;
          int b1 = buffer[i + 1] - '0';
          if (buffer[i + 1] >= 'A' && buffer[i + 1] <= 'F')
            b1 = (buffer[i + 1] - 'A') + 10;
          int b = 16 * b0 + b1;
          // printf("%c%c=%d\n", buffer[i], buffer[i+1],b);
          iq_data.push_back((unsigned char)b);
        }
      } else {
        for (int i = 0; i < strlen(buffer); i += 8) {
          int b = 0;
          int st = 128;
          for (int j = 0; j <= 7; j++) {
            int b1 = buffer[i + j] - '0';
            if (buffer[i + j] >= 'A' && buffer[i + j] <= 'F')
              b1 = (buffer[i + j] - 'A') + 10;
            b = b + st * b1;
            st = st / 2;
          }
          // printf("%c%c=%d\n", buffer[i], buffer[i+1],b);
          iq_data.push_back((unsigned char)b);
        }
      }

    } else if (l > 10000)
      break;
  }
  fclose(f);

  // send it to gl widget
  emit newRawDataSignal(iq_data.data(), l);
}

void TestDialog::on_pushButtonClear_clicked() {}

class PidScanTh : public QThread {
private:
  PidScan *ps;

public:
  PidScanTh(PidScan *ps) : ps(ps) {}
  void run() { ps->start(); }
};

QColor colorByNumber(int gi) {
  float max = 50.0;
  QColor color(20 + 200 / max * gi, 70 * (1.6 - gi / max), 150, 150);
  return color;
}

void TestDialog::on_pushButtonStartPID_clicked() {

  QVector<double> nv;

  for (auto b : bars) {
    b->setData(nv, nv);
  }

  bars.clear();

  int trans_id = ui->comboBoxFreqPID->currentIndex();
  if (!this->TuneToTranspId(trans_id))
    return;

  int max_steps = 100;
  int max_bit_rate = 4000;

  // prepare plot
  QCustomPlot *customPlot = ui->plotPID;

  customPlot->clearItems();
  customPlot->clearGraphs();

  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // set a more compact font size for bottom and left axis tick labels:
  customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  // set axis labels:
  customPlot->xAxis->setLabel("Step");
  customPlot->yAxis->setLabel("Bitrate (MB/s)");
  // make top and right axes visible but without ticks and labels:
  customPlot->xAxis2->setVisible(true);
  customPlot->yAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setTickLabels(false);
  // set axis ranges to show all data:
  customPlot->xAxis->setRange(0, max_steps);
  customPlot->yAxis->setRange(0, max_bit_rate);
  // show legend with slightly transparent background brush:
  customPlot->legend->setVisible(true);
  customPlot->legend->setWrap(15);
  customPlot->legend->clearItems();

  QFont fnt = customPlot->legend->font();
  fnt.setPointSize(8);
  customPlot->legend->setFont(fnt);
  customPlot->legend->setBrush(QColor(255, 255, 255, 150));

  ui->plot->axisRect()->setupFullAxesBox(true);
  ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  // run pidscan

  PidScan *scan = new PidScan(device, "", transponder, false);
  PidScanTh *scant = new PidScanTh(scan);

  should_collect_data = true;
  should_save_data = ui->checkBoxSaveTS->isChecked();

  scant->start();

  int pids_count = 0;
  QVector<double> ticks;
  QVector<QVector<double>> plot_data;
  QVector<int> all_PIDs;

  int s = 0;

  const int sleep_interval = 1000;

  while (true) {
    if (stopped_analize)
      break;
    QApplication::processEvents();
    QThread::msleep(sleep_interval);

    if (s == max_steps) {
      s = 0;
      ticks.clear();
      plot_data.clear();
      plot_data.resize(pids_count);
    }

    ticks.push_back(s);

    qDebug() << "-----------------";

    QVector<int> all_new_PIDs;

    // check for new pids
    int sum = 0;
    for (auto k : dvb_stat.keys()) {
      if (!all_PIDs.contains(k))
        all_new_PIDs.push_back(k);
      auto v = dvb_stat.value(k);
      sum += v;
      qDebug() << "[stat] pid = " << k << " len = " << v;
    }
    qDebug() << "bitrate: " << sum;
    qDebug() << "-----------------";

    // add new pids
    for (auto pid : all_new_PIDs) {
      QCPBars *bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
      bar->setName("pid " + QString::number(pid));
      bar->setAntialiased(false);
      // bar->setStackingGap(1);
      bar->setPen(QPen(colorByNumber(pids_count).lighter(200)));
      bar->setBrush(colorByNumber(pids_count));
      if (pids_count != 0)
        bar->moveAbove(bars[pids_count - 1]);

      bars.push_back(bar);

      pids_count++;
      plot_data.resize(pids_count);
      plot_data[pids_count - 1].resize(s);
      plot_data[pids_count - 1].fill(0); // in the past, it was 0
      all_PIDs.append(pid);
    }

    // copy data to graph
    for (int i = 0; i < pids_count; i++) {
      plot_data[i].resize(s + 1);
      int pid = all_PIDs[i];

      plot_data[i][s] = dvb_stat.value(pid) / sleep_interval;
    }

    // clear data
    should_collect_data = false;
    for (auto k : dvb_stat.keys()) {
      dvb_stat[k] = 0;
    }
    for (auto k : dvb_stat.keys()) {
      dvb_stat[k] = 0;
    }
    should_collect_data = true;

    for (int i = 0; i < pids_count; i++) {
      bars[i]->setData(ticks, plot_data[i]);
    }

    QApplication::processEvents();
    customPlot->replot();

    s++;
  }

  should_collect_data = false;
  should_save_data = false;

  device->release();

  // save all we collect if we want it
  if (ui->checkBoxSaveTS->isChecked())
    for (auto k : dvb_char_data.keys()) {
      auto v = dvb_char_data.value(k);
      QString fname = QDir::homePath() + "/pid_" + QString::number(k) + ".dvb";
      FILE *f = fopen(fname.toLocal8Bit(), "w");
      fwrite(v.constData(), v.size(), 1, f);
      fclose(f);
    }
}

void TestDialog::on_pushButtonStopPID_clicked() { stopped_analize = true; }
