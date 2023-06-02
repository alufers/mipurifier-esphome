## ESPHome for various Xiaomi Air Purification Devices

<p align="center"><img src="https://user-images.githubusercontent.com/36965186/219659691-6d8e733f-ae2f-4480-80ee-f5e90c9bd8f8.jpeg" width="400"></p>

There are two separate microcontrollers inside the device: one STM32F4 which actually controls the device, and one ESP32 which only acts as a gateway to the internet/local network. This config replaces the firmware running on the ESP32 and implements the proprietary serial protocol used to communicate with the STM32.

### Features:
- turning the purifier on/off
- exposing PM2.5, humidity, temperature, and remaining filter life as sensor values
- switching modes
- controlling speed in manual mode
- changing various settings (display brightness, beeper, lock)
- sending custom commands using the `esphome.mipurifier_send_command` service (e.g. sending `down set_properties 5 1 false` disables beeping)

### FAQ
#### Does this remove the `Filter remaining 0%` warning?
Flashing this firmware does NOT change anything about how the remaining filter life is read/managed. However you can easily reset your filter to continue using it by following [this video](https://www.youtube.com/watch?v=usiX_J9Yy2o) for example.

#### Is my air purifier supported?
The following devices are known to work with this library (this list is not exhaustive)
- Air Purifier 3 (2019) (zhimi.airpurifier.ma4)
- Air Purifier 3H (2019) (zhimi.airpurifier.mb3)

Please don't hesitate to expand this list by opening a pull request if your device also works with this library! Make sure to backup the original firmware on your device's ESP32 before flashing ESPHome.

### Flashing
Connect any USB-to-serial adapter and power on the ESP32 while `GPIO0` and `GPIO2` are pulled low.
<image src="https://user-images.githubusercontent.com/36965186/218741519-1f2bf1ba-cb44-488f-9ee8-803aed071dc3.jpeg" width=500>
