/** this file contains all functions required for OLED display operation */

#include "ch.h"
#include "hal.h"
#include "board.h"

CC_ALIGN_DATA(32) static uint8_t txbuf[128];
CC_ALIGN_DATA(32) static uint8_t rxbuf[128];

/**
 * @brief   Sets the reset line to the OLED low for 500 ms
 */
void oledReset(void){
  palClearLine(LINE_LCD_RESET_L);
  chThdSleepMilliseconds(500);
  palSetLine(LINE_LCD_RESET_L);
}

/**
 * @brief   Sends a single byte command to the OLED
 */
void oledCommand(uint8_t com){
  static uint8_t command[1];
  command[0] = com;
  spiSelect(&SPID1);                /** activate SPI chip select    */
  palClearLine(LINE_LCD_DAT_COMM);  /** set command bit to OLED     */
  spiSend(&SPID1, 1, command);      /** send command                */
  spiUnselect(&SPID1);              /** deactivate SPI chip select  */
}

/**
 * @brief   Sends a single byte data to the OLED
 */
void oledData(uint8_t dat){
  static uint8_t data[1];
  data[0] = dat;
  spiSelect(&SPID1);                /** activate SPI chip select    */
  palSetLine(LINE_LCD_DAT_COMM);    /** set data bit to OLED        */
  spiSend(&SPID1, 1, data);         /** send command                */
  spiUnselect(&SPID1);              /** deactivate SPI chip select  */
}

/**
 * @brief   sends data buffer to display RAM
 */
void oledWriteRam(uint8_t *data){
  oledCommand(0x5C);                /** activate writing to display RAM */
  uint8_t i; //counter variable
  for (i=0;i<sizeof(data);i++){
    oledData(data[i]);
  }
  oledCommand(0x5D);                /** disable writing to display RAM (switch to read from RAM) */
}

/**
 * @brief   Sends a command/data set to the OLED for initialization
 */
void oledInit(void){
  /** initialisation of the OLED */
  //oledCommand(0xFD);  oledData(0x12);    // set oledCommand unlock, default after reset: 0x12. 0x16 will lock all commands except 0xFD

  oledCommand(0xA0); oledData(0x14); oledData(0x11); //horizontal address increment, column address remap-disabled, enable Nibble Re-Map, scan from COM[N-1]; Dual COM Line Mode (0x11), default 0x01
  //oledCommand(0xA1); oledData(0x00);    // RAM start display start line to 0, default after reset: 0x00
  //oledCommand(0xA2); oledData(0x00);    //set display offset to 0 (vertical scroll), default after reset: 0x00
  //oledCommand(0xA6);                    //set normal display mode, default after reset (0xA7 is inverse mode)
  oledCommand(0xA9);                    //no partial mode/exit partial mode
  //oledCommand(0xAB); oledData(0x01);    //function select, enable internal VDD, default after reset: 0x01

  //oledCommand(0xB1); oledData(0xE8);    //set phase length: Phase 2: 14 DCLKs ("E"), Phase 1: 9 DCLKs ("8")
  oledCommand(0xB3); oledData(0x91);    //set display clock divide ratio to 135 Frames/sec
  oledCommand(0xB4); oledData(0xA0); oledData(0xFD);//enable VSL extern ("0xA0"), enhanced low GS quality ("0xFD")
  oledCommand(0xB5); oledData(0x00);    //disable GPIO[1,0] input
  oledCommand(0xB6); oledData(0x08);    //second pre charge period, 8 DCLKs
  oledCommand(0xB8);                    //set gray scale table, values must increase, value is in DCLKs
      oledData(0x00);   // grey level 1
      oledData(0x00);   // grey level 2
      oledData(0x00);   // grey level 3
      oledData(0x03);   // grey level 4
      oledData(0x06);   // grey level 5
      oledData(0x10);   // grey level 6
      oledData(0x1D);   // grey level 7
      oledData(0x2A);   // grey level 8
      oledData(0x37);   // grey level 9
      oledData(0x46);   // grey level 10
      oledData(0x58);   // grey level 11
      oledData(0x6A);   // grey level 12
      oledData(0x7F);   // grey level 13
      oledData(0x96);   // grey level 14
      oledData(0xB4);   // grey level 15
  oledCommand(0x00);    // enable grey scale table
  //oledCommand(0xB9);    // set default grey scale table
  oledCommand(0xBB); oledData(0x1F);    //pre charge voltage 0.6 * Vcc
  oledCommand(0xBE); oledData(0x07);    /// VCOMH 0.86 * Vcc, default after reset: 0x04 (0.8 * Vcc)

  oledCommand(0xC1); oledData(0xFF);    /// set contrast current max. Range: 0x00..0xFF
  //oledCommand(0xC7); oledData(0x0F);    /// set master contrast current, default after reset: 0x0F, range: 0x00..0x0F
  oledCommand(0xCA); oledData(0x3F);    /// set multiplex ratio to 64 (63d), default after reset: 0x7F (127d) = 128, range: 0x0F..0x7F (15d..127d) 16..128MUX

  //oledCommand(0xD1); oledData(0x82); oledData(0x20); // enhance driving scheme capability, default after reset: 0xA2, 0x20

  chThdSleepMilliseconds(1); //stabilize VDD

  oledCommand(0xAF);        //display on
  chThdSleepMilliseconds(200); //stabilize VDD
}

/**
 * @brief   Sends the window size to the OLED in horizontal mode
 */
void oledInitWindow(uint8_t startcol, uint8_t stopcol, uint8_t startrow, uint8_t stoprow){
  oledCommand(0x15);    oledData(28+startcol);  oledData(28+stopcol);   /** column start and end address. Range: 0..119 */
  oledCommand(0x75);    oledData(startrow);     oledData(stoprow);      /** row start and end adderss. Range: 0..127    */
  oledCommand(0x5C); /** write data to RAM */
  chThdSleepMilliseconds(50);
}
