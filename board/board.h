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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the LeafLabs Maple Mini.
 */

/*
 * Board identifier.
 */
#define BOARD_HPS_STM32_MK1
#define BOARD_NAME              "HPS STM23 MK1"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F103xB

/*
 * IO pins assignments
 *
 * numbering is sorted by onboard/connectors, as from the schematics in
 * https://github.com/leaflabs/maplemini
 */

/* on-board */

/** LEDs on board are physically swapped to the desired output */
#define LINE_LED_A          PAL_LINE(GPIOB, 1)     // pin 27
#define LINE_LED_B          PAL_LINE(GPIOB, 0)     // pin 26

/** buttons are numbered according to their physical location and desired function */
#define LINE_BUTTON_A       PAL_LINE(GPIOB, 13)    // pin 34
#define LINE_BUTTON_B       PAL_LINE(GPIOB, 14)    // pin 35
#define LINE_BUTTON_L       PAL_LINE(GPIOB, 12)    // pin 33
#define LINE_BUTTON_R       PAL_LINE(GPIOB, 11)    // pin 30
#define LINE_BUTTON_MENU    PAL_LINE(GPIOB, 15)    // pin 36
#define LINE_ROTARY_A       PAL_LINE(GPIOC, 11)    // pin 52
#define LINE_ROTARY_B       PAL_LINE(GPIOC, 10)    // pin 51

#define LINE_OSC_IN         PAL_LINE(GPIOD,  0)    // pin 5
#define LINE_OSC_OUT        PAL_LINE(GPIOD,  1)    // pin 6

/** USART */
#define LINE_USART1_RX      PAL_LINE(GPIOA, 10)    // pin 43
#define LINE_USART1_TX      PAL_LINE(GPIOA,  9)    // pin 42

/** SPI, connected to LCD/OLED */
#define LINE_SPI1_NSS       PAL_LINE(GPIOA,  4)    // pin 20
#define PAD_SPI1_NSS        4                      // alternative if PAD is used for SPI
#define LINE_SPI1_SCK       PAL_LINE(GPIOA,  5)    // pin 21
#define LINE_SPI1_MOSI      PAL_LINE(GPIOA,  7)    // pin 15

/** defines for u8g2 library    */
#define LCD_CS_GPIO_Port    GPIOA
#define LCD_CS_Pin          PAD_SPI1_NSS

/** I²C, connected to EEPROM/alternate LCD/OLED */
#define LINE_I2C_SCL        PAL_LINE(GPIOB,  6)    // pin 58
#define LINE_I2C_SDA        PAL_LINE(GPIOB,  7)    // pin 59

/** USB */
#define LINE_USBDP          PAL_LINE(GPIOA, 12)    // pin 45
#define LINE_USBDM          PAL_LINE(GPIOA, 11)    // pin 44

/** LCD */
#define LINE_LCD_BL_EN      PAL_LINE(GPIOA,  0)    // pin 14
#define LINE_LCD_EN         PAL_LINE(GPIOA,  1)    // pin 15
#define LINE_LCD_DAT_COMM   PAL_LINE(GPIOA,  2)    // pin 16
#define LINE_LCD_RESET_L    PAL_LINE(GPIOA,  3)    // pin 17


/** ADC */
#define LINE_ADC_CUR_A      PAL_LINE(GPIOC,  0)    // pin 8
#define LINE_ADC_VOL_A      PAL_LINE(GPIOC,  1)    // pin 9
#define LINE_ADC_CUR_B      PAL_LINE(GPIOC,  2)    // pin 10
#define LINE_ADC_VOL_B      PAL_LINE(GPIOC,  3)    // pin 11

/** PWM */
#define LINE_PWM_CUR_A      PAL_LINE(GPIOC,  7)    // pin 38
#define LINE_PWM_VOL_A      PAL_LINE(GPIOC,  6)    // pin 37
#define LINE_PWM_CUR_B      PAL_LINE(GPIOC,  9)    // pin 40
#define LINE_PWM_VOL_B      PAL_LINE(GPIOC,  8)    // pin 39

#define LED_ON              PAL_HIGH
#define LED_OFF             PAL_LOW
#define BUTTON_PRESSED      PAL_HIGH
/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0..4       - Normal output, PA4 SPI NSS
 * PA5,7        - Alternate output SPI SCK, MOSI
 * PA9          - Output USART
 * PA10         - Input USART
 * PA11,12      - USB
 * PA13,14      - SW DIO, CLK
 */
//#define VAL_GPIOACRL            0xB8B33333      /*  PA7...PA0 */
#define VAL_GPIOACRL            0xB8B33333      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x888884B8      /* PA15...PA8 */
#define VAL_GPIOAODR            0xffffffff

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0,1    - Push Pull output  (LED).
 * PB6,7    - open drain "I2C"
 * PB8,9    - "debug" outputs
 * PB11..15 - normal input, pull-down externally provided
 */
#define VAL_GPIOBCRL            0x55888833      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x44444338      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC0..3   - analog input
 * PC6..9   - alternate output (Timer3 Ch1..4, remap required)
 * PC10,11  - normal input, rotary encoder
 */
#define VAL_GPIOCCRL            0xBB880000      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x888844BB      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0      - Normal input (XTAL).
 * PD1      - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
