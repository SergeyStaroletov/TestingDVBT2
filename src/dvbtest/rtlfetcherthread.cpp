#include "rtlfetcherthread.h"
#include <QDebug>

bool RTLFetcherThread::setup() {
  uint32_t samp_rate = DEFAULT_SAMPLE_RATE;
  int gain = 0;
  int ppm_error = 0;
  int dev_index = verbose_device_search("0");

  if (dev_index < 0) {
    return false;
  }

  int r = rtlsdr_open(&dev, (uint32_t)dev_index);
  if (r < 0) {
    qDebug() << "Failed to open rtlsdr device!\n";
    return false;
  }
  /* Set the sample rate */
  verbose_set_sample_rate(dev, samp_rate);
  /* Set the frequency */
  verbose_set_frequency(dev, freq);
  if (0 == gain) {
    /* Enable automatic gain */
    verbose_auto_gain(dev);
  } else {
    /* Enable manual gain */
    gain = nearest_gain(dev, gain);
    verbose_gain_set(dev, gain);
  }
  verbose_ppm_set(dev, ppm_error);
  /* Reset endpoint before we start reading from it (mandatory) */
  verbose_reset_buffer(dev);

  return true;
}

void RTLFetcherThread::run() {

  int n_read;
  uint32_t out_block_size = 8000000; /*DEFAULT_BUF_LENGTH;*/

  char *buf_small = (char *)malloc(out_block_size * 2);

  *should_die = 0;

  // a transaction to obtain data from the rtl device
  while (!(*should_die)) {

    sem_buf->lock();
    int current_pos = 0;
    bytes_to_read = 2000000;

    bool need_fin = false;
    while (!need_fin) { // until we fail or get a desired size

      int r = rtlsdr_read_sync(dev, buf_small, out_block_size, &n_read);
      if (r < 0) {
        qDebug() << "WARNING: sync read failed.\n";
        break;
      }

      if ((bytes_to_read > 0) && (bytes_to_read < (uint32_t)n_read)) {
        n_read = bytes_to_read;
        need_fin = true;
      }

      // we got n_read bytes = append to the buffer
      memccpy(buf, buf_small, current_pos, n_read);
      current_pos += n_read;

      if ((uint32_t)n_read < out_block_size) {
        qDebug() << "Short read, samples lost, exiting!\n";
        break;
      }

      if (bytes_to_read > 0)
        bytes_to_read -= n_read;
    }
    *this->buf_size = current_pos;

    qDebug() << "We got buffer with size " << current_pos
             << " from the device...\n";

    sem_buf->unlock();
  }

  rtlsdr_close(dev);
  free(buf_small);
}
