/** this file contains all SPI related code and functions */

#include "ch.h"

#if SPI_SUPPORTS_CIRCULAR == TRUE
/*
 * SPI callback for circular operations.
 */
void spi_circular_cb(SPIDriver *spip) {

#if SPI_SUPPORTS_CIRCULAR == TRUE
    if (palReadLine(LINE_BUTTON_L) == BUTTON_PRESSED) {
      osalSysLockFromISR();
      spiAbortI(&SPID1);
      osalSysUnlockFromISR();
    }
#endif

  if (spiIsBufferComplete(spip)) {
    /* 2nd half.*/
#if defined(LINE_LED_A)
    palWriteLine(LINE_LED_A, LED_OFF);
#endif
  }
  else {
    /* 1st half.*/
#if defined(LINE_LED_A)
    palWriteLine(LINE_LED_A, LED_ON);
#endif
  }
}
#endif

/*
 * SPI error callback, only used by SPI driver v2.
 */
void spi_error_cb(SPIDriver *spip) {

  (void)spip;

  chSysHalt("SPI error");
}

/*
 * Circular SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig c_spicfg = {
  .circular         = true,
  .slave            = false,
  .data_cb          = spi_circular_cb,
  .error_cb         = spi_error_cb,
  .ssline           = LINE_SPI1_NSS,
//  .ssport           = GPIOA,
//  .sspad            = GPIOA_SPI1NSS,
  .cr1              = 0U,
  .cr2              = 0U
};

/*
 * Maximum speed SPI configuration (9MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig hs_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = spi_error_cb,
  .ssline           = LINE_SPI1_NSS,
  .cr1              = SPI_CR1_BR_0, // divide STM32_PPRE1 by 4
  .cr2              = 0U
};

/*
 * Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig ls_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = spi_error_cb,
  .ssline           = LINE_SPI1_NSS,
  .cr1              = SPI_CR1_BR_2 | SPI_CR1_BR_1,
  .cr2              = 0U
};
