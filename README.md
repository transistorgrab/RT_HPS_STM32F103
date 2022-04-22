# RT_HPS_STM32F103
a simple STM32F103 board with some LEDs, buttons, rotary encoder and a SSD1322 based OLED.
the project is aimed to be controlling a small power supply.
this code is currently (2022-04-22) in its early stages where I try to get everything running.

## Issues so far:
* the SSD1322 driver from uGFX is missing its driver.mk file.
* the driver also has an error in its memory addressing and is violating its allocated memory area.

This line (216) in gdisp_lld_SSD1322.c 

    ram = RAM(g)+xyaddr(x,y);

needs to be changed into 

    ram = RAM(g)+xyaddr(x/2,y);
    
at least that is the status today (2022-04-22).
    
