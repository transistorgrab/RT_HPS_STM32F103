# RT_HPS_STM32F103
a simple STM32F103 board with some LEDs, buttons, rotary encoder and a SSD1322 based OLED.
the project is aimed to be controlling a small power supply.
this code is function complete.

## Issues so far:
* the SSD1322 driver from uGFX is missing its driver.mk file.
* the driver also has an error in its memory addressing and is violating its allocated memory area.

added diff for ugfx for the SSD1322 driver to function correctly.

