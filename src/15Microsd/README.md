Every once in a while you come across an idea, where you need for a way to store large amount of log data and other information for your Arduino project, for example a GPS logger.

The solution is to use what’s found in every digital camera and mp3 player: Flash Cards! often called SD or micro SD cards. Their ability to pack GigaBytes of data into a space smaller than a coin made them indispensable thing in our life.

Hardware Overview
The micro SD card module contains two main components that make it undoubtedly easy to add data logging to your next Arduino project:

Micro SD TF Card Module module contains level shifter and regulator
The operating voltage of any standard micro SD Cards is 3.3 V. So we cannot directly connect it to circuits that use 5V logic. In fact, any voltages exceeding 3.6V will permanently damage the micro SD card. That’s why; the module has an onboard ultra-low dropout regulator that will convert voltages from 3.3V – 6V down to ~3.3V.
There’s also a 74LVC125A chip on the module which converts the interface logic from 3.3V-5V to 3.3V. This is called logic level shifting. That means you can use this board to interact with both 3.3V and 5V microcontrollers like Arduino.
There are actually two ways to interface with micro SD cards – SPI mode and SDIO mode. SDIO mode is way faster and is used in mobile phones, digital cameras etc. But it is more complex and requires signing non-disclosure documents. For that reason, hobbyist like us will likely never encounter SDIO mode interface code. Instead, every SD card module is based on ‘lower speed & less overhead’ SPI mode that is easy for any microcontroller to use.

