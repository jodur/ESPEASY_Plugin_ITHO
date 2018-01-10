# ESPEASY_Plugin_ITHO
Plugin for ESPEasy regarding a ITHO Fan remote

A CC1101 868Mhz transmitter is needed (available on ebay or aliexpress)

This plugin is using the library from: https://github.com/supersjimmie/IthoEcoFanRFT/tree/master/Master/Itho 
made by 'supersjimmie' and 'klusjesman'

For more info see also: https://gathering.tweakers.net/forum/list_messages/1690945 

|CC11xx pin    |ESP pins|Description                                        |
|:-------------|:-------|:--------------------------------------------------|
|1 - VCC       |VCC     |3v3                                                |
|2 - GND       |GND     |Ground                                             |
|3 - MOSI      |13=D7   |Data input to CC11xx                               |  
|4 - SCK       |14=D5   |Clock pin                                          |
|5 - MISO/GDO1 |12=D6   |Data output from CC11xx / serial clock from CC11xx |
|6 - GDO2      |04=D1*  |output as a symbol of receiving or sending data    |
|7 - GDO0      |        |output as a symbol of receiving or sending data    |
| 8 - CSN      |15=D8   |Chip select / (SPI_SS)                             |

*Note: GDO2 is used as interrupt pin for receiving and is configurable in the plugin

Not recommended pins for intterupt:
- Boot pins D3(GPIO0) and D4 (GPIO2) 
- Pin with no interrupt support: D0 (GPIO16)

## List of commands:

1111 to join ESP8266 with Itho ventilation unit

9999 to leaveESP8266 with Itho ventilation unit

0 - set Itho ventilation unit to standby

1 - set Itho ventilation unit to low speed

2 - set Itho ventilation unit to medium speed

3 - set Itho ventilation unit to high speed

4 - set Itho ventilation unit to full speed

13 - set itho to high speed with hardware timer (10 min)

23 - set itho to high speed with hardware timer (20 min)

33 - set itho to high speed with hardware timer (30 min)


## List of States:

1 - Itho ventilation unit to lowest speed

2 - Itho ventilation unit to medium speed

3 - Itho ventilation unit to high speed

4 - Itho ventilation unit to full speed

13 -Itho to high speed with hardware timer (10 min)

23 -Itho to high speed with hardware timer (20 min)

33 -Itho to high speed with hardware timer (30 min)

In the plugin you are able to define 3 RF device ID's for the existing RF remote controls the plugin is listening to, to update the state of the fan.
You are able to capture the id of you RF remote, by setting the log settings to 3, in the advanced settings menu. After pressing a button, you will see the ID (23 chars) of the RF in the log. Use ID with '-'. 
#### example ID: 11-22-33-44-55-66-77-88

In case a timerfunction is called (timer 1..3), an internal timer is running as estimate for the elapsed time.

The plugin will publish MQTT topics as they change. The aquisition cycle time should be used as a state update cycle time.
In case a topic doesnT change the cycle time is used for cyclic update. It is recommended to set this to higher values: for example to 60s

For the lazy people or people with no codeskills a binary is added for ESPEASY with all stable plugins including this plugin

When using a Wemos D1 mini, you have to remove D1 and D2 from I2C on the hardware page, because one of the pins must be used as Intterupt pin. See note above concerning the interrupt pins.
