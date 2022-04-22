/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "board.h"
#include "spi.c"
#include "oled.c"
#include "stdlib.h"
#include "chprintf.h"
#include "string.h"
//#include "u8g2conf.h"
//#include "u8g2/csrc/u8g2.h"
#include "gfx.h"

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    bool key_pressed = palReadLine(LINE_BUTTON_R) == BUTTON_PRESSED;
    systime_t time = key_pressed ? 250 : 500;
    palToggleLine(LINE_LED_B);
    chThdSleepMilliseconds(time);
  }
}

/*
 * Serial thread.
 */
THD_WORKING_AREA(waThread3, 128);
THD_FUNCTION(Thread3, arg) {

  (void)arg;
  chRegSetThreadName("hello");
  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  while (true) {
    chnWrite(&SD1, (const uint8_t *)"Hello World!\r\n", 14);
    chThdSleepMilliseconds(2000);
  }
}

/*
 * OLED thread
 */
THD_WORKING_AREA(waThreadOLED, 512);
THD_FUNCTION(ThreadOLED, arg) {

  (void)arg;
  chRegSetThreadName("OLED");


  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  // setup display with u8g2 library
  //u8g2_t u8g2;    // structure for display data
  //u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8g2_gpio_and_delay_stm32);

  spiStart(&SPID1, &hs_spicfg); /* SPI needs to be initialized before setting up the display */
  gfxInit();



  //u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  //u8g2_SetPowerSave(&u8g2, 0); // wake up display

//  gFont font = gdispOpenFont("DejaVuSans12_aa");
  gdispSetPowerMode(powerOn);
//  gdispSetOrientation(GDISP_ROTATE_180);
  int width = gdispGetWidth();
  int height = gdispGetHeight();
  //chThdSleepMilliseconds(1000);

  //u8g2_DrawLine(&u8g2, 50,50, 100, 100);
  //u8g2_SendBuffer(&u8g2);

// native code
//  palSetLine(LINE_SPI1_NSS);    /** set SPI chip select inactive */
//  palSetLine(LINE_LCD_EN);      /** switch on power supply for OLED */
//  oledReset();      /** engage Reset of OLED */
//
//  spiStart(&SPID1, &hs_spicfg);  /* Setup transfer parameters.       */
//  oledInit();
//  oledInitWindow(0, 119-28, 0, 127);

  while (true) {
 //   oledWriteRam(txbuf);   /* write txbuf to display RAM   */
    chThdSleepMilliseconds(500);
    //gdispDrawString(2,2,"HPS Startup",font,GFX_GREY);
    gdispDrawLine(5, 30, width - 50, height - 40, GFX_WHITE);
  }
}


/*
 * Application entry point.
 */
int main(void) {
  unsigned int cycles = 0;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();    // calls boardInit() at last
  chSysInit();

//  spiStart(&SPID1, &hs_spicfg); /* SPI needs to be initialized before setting up the display */
//  gfxInit();

  /*
   * Creates the blinker and serial thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO, Thread3, NULL);
  chThdCreateStatic(waThreadOLED, sizeof(waThreadOLED), NORMALPRIO, ThreadOLED, NULL);
  /*
   * Prepare transmit pattern.
   */
  for (unsigned i = 0; i < sizeof(txbuf); i++){
    txbuf[i] = (uint8_t)i;
  }
  cacheBufferFlush(&txbuf[0], sizeof txbuf);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
    //txbuf[i] = (int) cycles*cycles;
    //cacheBufferFlush(&txbuf[0], sizeof txbuf);
    cycles ++;
  }
  return 0;
}
