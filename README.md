## Christmas lights
Here we drive [WS2812](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf) based individually addressable LED chain with [Esp32](https://www.espressif.com/en/products/socs/esp32) board in FreeRTOS; using the [Esp-Idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) SDK.
Project aim is to support multiple light effects, and possibly choose them by TCP communication with a remote ( Telnet ) console supporting simple commands, or with digital input ( button(s) ).

**UPDATE**

Unfortunately [my board](https://www.amazon.it/V1-0-0-ESP32-S2FN4R2-sviluppo-Compatibile-MicroPython/dp/B0DCBXDBRJ/ref=asc_df_B0DCBXDBRJ/?tag=googshopit-21&linkCode=df0&hvadid=711155295770&hvpos=&hvnetw=g&hvrand=8573302761926349073&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9205939&hvtargid=pla-2368624922545&mcid=7796b3108ecf38a5b09b0fd0f1a205ff&gad_source=1&th=1) does not support driving RMT ( used for LED chain ) and neither WiFi nor using additional RMT channel for IR receiver. I left the attempt in the code for further improvements.

### Disclaimer
The following section is not a tutorial on building an electronic circuit. Although the design poses no harmful risks with a 5V power supply, you may still damage yourself, your equipment, or the components if you are not experienced with such projects.

### Schematic
Here below the schematic diagram:

![schema](https://github.com/FelicePollano/christmas-lights/blob/main/images/circuit.png)

Circuit is optimized for minimize wiring, I used negate logic to "ground" the button instead of wiring out the 3.3V line. Thanks to internal pull up resistor available on GPIO pin, no external components are used. Fortunately, my WS2812B chain works well even with 3.3 V logic, this sould not be the case according to official docs. In case you experience troubles in driving the data signal, I suggest this [ single BJT non inverting level shifter:](https://electronics.stackexchange.com/q/82104/11079).
![](https://i.sstatic.net/EUVMW.png)

### Power supply
A 5V DC power supply is required, which is widely available. Regarding current: in theory, each LED could draw up to 60mA when displaying bright white. However, this scenario is unlikely in practice. For example, I have a 600-LED strip that could potentially draw up to 36A, but with the lighting effects in the code, I've never exceeded 4-5A.
