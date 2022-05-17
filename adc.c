/** this file contains all functions required for ADC operation */

#include "ch.h"
#include "hal.h"
#include "board.h"

/* ADC stuff    */
#define ADC_GRP1_NUM_CHANNELS   4
#define ADC_GRP1_BUF_DEPTH      8

static adcsample_t adcSamplesBuf[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

uint16_t adcVolA, adcVolB, adcCurA, adcCurB;
uint16_t adcChannels[ADC_GRP1_NUM_CHANNELS]; //adcCurA, adcVolA, adcCurB, adcVolB
uint32_t samplesSum = 0; // temporary holds the sum of sample values for a channel. ADC max value = 4095

/*
 * ADC streaming callback.
 */
static void adcCallback(ADCDriver *adcp) {

  if (adcIsBufferComplete(adcp)) {
    for (int channel = 0; channel < ADC_GRP1_NUM_CHANNELS; channel++){  // iterate over all channels
      samplesSum = 0;   // reset sum variable
      for (int sample = 0; sample < ADC_GRP1_BUF_DEPTH; sample++){      // iterate over all samples
        samplesSum += adcSamplesBuf[channel * sample];
      }
      adcChannels[channel] = samplesSum/ADC_GRP1_BUF_DEPTH; // calculate the average for this channel over No. of samples
    }
    adcCurA = adcChannels[0];
    adcVolA = adcChannels[1];
    adcCurB = adcChannels[2];
    adcVolB = adcChannels[3];
    chThdSleepMilliseconds(50);
  }
}

static void adcErrorCallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 8 samples of 1 channel, SW triggered.
 * Channels:    IN10.
 */
static const ADCConversionGroup adcgrpcfg1 = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  NULL,//adcCallback,
  adcErrorCallback,
  0, 0,//ADC_CR2_SWSTART,//|ADC_CR2_TSVREFE,                         /* CR1, CR2 */
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_7P5)|       // current A    /* SMPR1 */
  ADC_SMPR1_SMP_AN11(ADC_SAMPLE_7P5)|       // voltage A
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_7P5)|       // current B
  ADC_SMPR1_SMP_AN13(ADC_SAMPLE_7P5),       // voltage B
  0,  /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,                            /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)|
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11)|
  ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12)|
  ADC_SQR3_SQ4_N(ADC_CHANNEL_IN13)
};
