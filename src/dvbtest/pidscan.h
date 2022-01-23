#ifndef PIDSCAN_H
#define PIDSCAN_H

#include "dvb/dvbchannel.h"
#include <dvb/dvbscan.h>

class AtscVctSection;
class DvbDescriptor;
class DvbDevice;
class DvbNitSection;
class DvbPatEntry;
class DvbPatSection;
class DvbPmtSection;
class PidScanFilter;
class DvbSdtEntry;
class DvbSdtSection;

class PidScan : public QObject {
  friend class PidScanFilter;
  Q_OBJECT
public:
  PidScan(DvbDevice *device_, const QString &source_,
          const DvbTransponder &transponder_, bool useOtherNit);
  PidScan(DvbDevice *device_, const QString &source_,
          const QList<DvbTransponder> &transponders_, bool useOtherNit);
  PidScan(DvbDevice *device_, const QString &source_,
          const QString &autoScanSource, bool useOtherNit);
  ~PidScan();

  void start();

signals:
  void foundChannels(const QList<DvbPreviewChannel> &channels);
  void scanProgress(int percentage);
  void scanFinished();

private slots:
  void deviceStateChanged();

private:
  enum State { ScanPat, ScanNit, ScanSdt, ScanPmt, ScanTune, ScanTuning };

  bool startFilter(int pid, DvbScan::FilterType type);
  void updateState();

  void processPat(const DvbPatSection &section);
  void processPmt(const DvbPmtSection &section, int pid);
  void processSdt(const DvbSdtSection &section);
  void processVct(const AtscVctSection &section);
  void processNit(const DvbNitSection &section);
  void processNitDescriptor(const DvbDescriptor &descriptor);
  void filterFinished(PidScanFilter *filter);

  DvbDevice *device;
  QString source;
  DvbTransponder transponder;
  bool isLive;
  bool isAuto;
  bool useOtherNit;

  // only used if isLive is false
  QList<DvbTransponder> transponders;
  int transponderIndex;

  State state;
  QList<DvbPatEntry> patEntries;
  int patIndex;
  QList<DvbSdtEntry> sdtEntries;
  QList<DvbPreviewChannel> channels;

  DvbBackendDevice::Scale scale;
  float snr;
  int transportStreamId;

  QList<PidScanFilter *> filters;
  int activeFilters;
};

#endif // PIDSCAN_H
