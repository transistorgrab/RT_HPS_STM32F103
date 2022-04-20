/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

//Optional
//#define SSD1322_USE_DMA

#ifndef SSD1322_USE_DMA
	#define SSD1322_USE_DMA			GFXOFF
#endif

static GFXINLINE void init_board(GDisplay *g) {
	(void) g;
    // As we are not using multiple displays we set g->board to NULL as we don't use it.
    g->board = 0;

}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, gBool state) {
	(void) g;
	(void) state;

    if(state)
        palClearLine(LINE_LCD_RESET_L);
    else
        palSetLine(LINE_LCD_RESET_L);
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
    spiAcquireBus(&SPID1);
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
	spiReleaseBus(&SPID1);
}


static GFXINLINE void write_cmd(GDisplay *g, gU8 cmd) {
	(void) g;
	(void) cmd;

    spiSelect(&SPID1);                  /** activate SPI chip select    */
	palClearLine(LINE_LCD_DAT_COMM);    /** set command bit to OLED     */
	spiSend(&SPID1, 1, &cmd);           /** send command                */
	spiUnselect(&SPID1);                /** deactivate SPI chip select  */

}

static GFXINLINE void write_data(GDisplay *g, gU8 data) {
	(void) g;
	(void) data;
    spiSelect(&SPID1);                  /** activate SPI chip select    */
	palSetLine(LINE_LCD_DAT_COMM);      /** set data bit to OLED        */
	spiSend(&SPID1, 1, &data);          /** send command                */
    spiUnselect(&SPID1);                /** deactivate SPI chip select  */
}

#if SSD1322_USE_DMA
	static GFXINLINE void write_data_DMA(GDisplay *g, gU8* data) {
		(void) g;
		(void) data;
	}
#endif	// Use DMA

#endif /* _GDISP_LLD_BOARD_H */
