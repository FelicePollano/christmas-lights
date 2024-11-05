## Christmas lights
Here we drive [WS2812](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf) based individually addressable LED chain with [Esp32](https://www.espressif.com/en/products/socs/esp32) board in FreeRTOS; using the [Esp-Idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) SDK.
Project aim is to support multiple light effects, and possibly choose them by TCP communication with a remote ( Telnet ) console supporting simple commands, or with digital input ( button(s) ).

**UPDATE**

Unfortunately [my board](https://www.amazon.it/V1-0-0-ESP32-S2FN4R2-sviluppo-Compatibile-MicroPython/dp/B0DCBXDBRJ/ref=asc_df_B0DCBXDBRJ/?tag=googshopit-21&linkCode=df0&hvadid=711155295770&hvpos=&hvnetw=g&hvrand=8573302761926349073&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9205939&hvtargid=pla-2368624922545&mcid=7796b3108ecf38a5b09b0fd0f1a205ff&gad_source=1&th=1) does not support driving RMT ( used for LED chain ) and neither WiFi nor using additional RMT channel for IR receiver. I left the attempt in the code for further improvements.
