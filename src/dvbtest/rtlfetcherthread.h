#ifndef RTLFETCHERTHREAD_H
#define RTLFETCHERTHREAD_H

#include <QMutex>
#include <QThread>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dvbtest/rtl_sdr/convenience/convenience.h"
#include "dvbtest/rtl_sdr/include/rtl-sdr.h"

// for rtl-sdr
#define DEFAULT_SAMPLE_RATE 2048000
#define DEFAULT_BUF_LENGTH (16 * 16384)
#define MINIMAL_BUF_LENGTH 512
#define MAXIMAL_BUF_LENGTH (256 * 16384)
#define BUF_RTL_SIZE 2000000

class RTLFetcherThread : public QThread {
public:
  RTLFetcherThread(uint32_t freq, char *buf, uint32_t *buf_size,
                   bool *should_die, QMutex *sem_buf)
      : freq(freq), buf(buf), sem_buf(sem_buf), buf_size(buf_size),
        should_die(should_die) {}

  bool setup();

  virtual void run();

private:
  uint32_t freq;
  char *buf;
  uint32_t *buf_size;
  bool *should_die;
  QMutex *sem_buf;
  int do_exit = 0;
  uint32_t bytes_to_read = 0;
  rtlsdr_dev_t *dev = NULL;
};

#endif // RTLFETCHERTHREAD_H
