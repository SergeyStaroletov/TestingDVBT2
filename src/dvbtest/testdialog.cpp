#include "testdialog.h"
#include "ui_testdialog.h"
#include <dvb/dvbliveview.h>
#include <dvb/dvbdevice.h>
#include <dvb/dvbtransponder.h>
#include <dvb/dvbscan.h>
#include <dvb/dvbbackenddevice.h>
#include <dvb/dvbconfig.h>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <stdio.h>

class ScanThread:public QThread {
public:
    ScanThread(DvbScan* scan) {this->scan = scan;}
private:
    DvbScan* scan;
    void run() {
        scan->start();
    }

};

TestDialog::TestDialog(QWidget *parent, DvbManager *manager) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);
    this->manager = manager;
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::on_pushButton_clicked()
{
    //
    DvbDevice * device = manager->getDeviceConfigs().at(1).device;

    const DvbDeviceConfig &it =  manager->getDeviceConfigs().at(1);

    foreach (const DvbConfig &config, it.configs) {
        if (config->name == "Terrestrial (T2)") {
            device = it.device;
            device->acquire(config.constData());
            qDebug() << config->name;
        }
    }


    if (device==NULL) {
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


    return;*/



    DvbT2Transponder *t = new DvbT2Transponder();
    memset(t, 0, sizeof(DvbT2Transponder));
    t->bandwidth = DvbT2Transponder::Bandwidth8MHz;
    t->frequency = 522000000;
    t->modulation = DvbT2Transponder::Qam256; //DvbT2Transponder::ModulationAuto;
    t->fecRateHigh = DvbT2Transponder::FecAuto;
    t->fecRateLow = DvbT2Transponder::FecNone;
    t->guardInterval = DvbT2Transponder::GuardIntervalAuto;
   // t->modulation = DvbT2Transponder::ModulationAuto;
    t->transmissionMode=DvbT2Transponder::TransmissionModeAuto;

    //qDebug() << t->toString();

    DvbTransponder transponder(DvbTransponderBase::DvbT2);

    DvbTransponder tt = transponder.fromString(t->toString());

    device->tune(tt);

    DvbScan* scan = new DvbScan(device, src, tt, false);


    ScanThread * th = new ScanThread(scan);
    th->start();

    FILE *f = fopen("/home/sergey/dvb.csv", "w");

    char gps_norm [] = "0;0;";

    while (true) {


        device->getProps(tt);

        DvbT2Transponder *dvb = tt.as<DvbT2Transponder>();

        switch(dvb->modulation) {
            case 0 : qDebug() << "QPSK\n";break;
            case 1: qDebug() << "Qam16\n"; break;
            case 2: qDebug() << "Qam64\n"; break;
            case 3: qDebug() << "Qam256\n"; break;
            default: qDebug() << "unknown mod\n";
        };

       float sig = device->getSignal(s);
       float snr = device->getSnr(s);

       qDebug() << "sig = " << sig << " snr = " << snr << "\n";

       char gp[100];
       memset(gp,0,sizeof(gp));
       FILE *gps = fopen("/home/sergey/gps.txt", "r");
       if (gps) fgets(gp,100,gps);
       fclose(gps);
       if (strlen(gp) > 0) {strcpy(gps_norm, gp);}

       qDebug() << "GPS: " << gps_norm << "\n";

       QDateTime date = QDateTime::currentDateTime();
       QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss.zzz");

       fprintf(f, "\"%s\";%s%d;%f;%f\n", formattedTime.toLocal8Bit().data(), gps_norm, device->isTuned(), - device->getSignal(s), device->getSnr(s));
       fflush(f);

       QThread::currentThread()->msleep(777);



    }




}
