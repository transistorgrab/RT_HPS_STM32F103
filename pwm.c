/** this file contains all pwm related code     */

#include "ch.h"
#include "hal.h"
#define TIM3MAX 4096UL   // 16 bit timer, but we use only 12 bits

#define PWM_CH_VOL_A &PWMD3, 0
#define PWM_CH_VOL_B &PWMD3, 2
#define PWM_CH_CUR_A &PWMD3, 1
#define PWM_CH_CUR_B &PWMD3, 3

PWMConfig pwmConf = {
  36000000,  /** 36 MHz PWM clock                                           */
  TIM3MAX,   /** Period. Resolution 12 bits = equal to ADC resolution. f_PWM ~ 8.8 kHz  */
  NULL,     /** period callback                                             */
  {
   {PWM_OUTPUT_ACTIVE_LOW, NULL},  /** CH1 mode and callback: Voltage A    */
   {PWM_OUTPUT_ACTIVE_LOW, NULL},  /** CH2 mode and callback: Current A    */
   {PWM_OUTPUT_ACTIVE_LOW, NULL},  /** CH3 mode and callback  Voltage B    */
   {PWM_OUTPUT_ACTIVE_LOW, NULL}   /** CH4 mode and callback  Current B    */
  },
  0,        /** control register 2              */
  0         /** DMA/interrupt enable register   */
};

