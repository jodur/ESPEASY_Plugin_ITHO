# ESPEASY_Plugin_ITHO
Plugin for ESPEasy regarding a ITHO Fan remote

This plugin is using the library from: https://github.com/supersjimmie/IthoEcoFanRFT/tree/master/Master/Itho 
made by 'supersjimmie' and 'klusjesman'

For more info see also: https://gathering.tweakers.net/forum/list_messages/1690945 

CC11xx pins    ESP pins Arduino pins  Description

  1 - VCC        VCC      VCC           3v3
  2 - GND        GND      GND           Ground
  3 - MOSI       13=D7    Pin 11        Data input to CC11xx
  4 - SCK        14=D5    Pin 13        Clock pin
  5 - MISO/GDO1  12=D6    Pin 12        Data output from CC11xx / serial clock from CC11xx
  6 - GDO2       04=D1    Pin 2?        output as a symbol of receiving or sending data
  7 - GDO0       ?        Pin  ?        output as a symbol of receiving or sending data
  8 - CSN        15=D8    Pin 10        Chip select / (SPI_SS)

Note: GDO2 is used as interrupt pin and is configurable in the plugin

Not recommended pins for intterupt:
- Boot pins D3(GPIO0) and D4 (GPIO2) 
- Pin with no interrupt support: D0 (GPIO16)

// List of commands:
// 1111 to join ESP8266 with Itho ventilation unit
// 9999 to leaveESP8266 with Itho ventilation unit
// 0 to set Itho ventilation unit to standby
// 1 - set Itho ventilation unit to low speed
// 2 - set Itho ventilation unit to medium speed
// 3 - set Itho ventilation unit to high speed
// 4 - set Itho ventilation unit to full speed
// 13 - set itho to high speed with hardware timer (10 min)
// 23 - set itho to high speed with hardware timer (20 min)
// 33 - set itho to high speed with hardware timer (30 min)

//List of States:

// 1 - Itho ventilation unit to lowest speed
// 2 - Itho ventilation unit to medium speed
// 3 - Itho ventilation unit to high speed
// 4 - Itho ventilation unit to full speed
// 13 -Itho to high speed with hardware timer (10 min)
// 23 -Itho to high speed with hardware timer (20 min)
// 33 -Itho to high speed with hardware timer (30 min)

In the plugin you are able to define 3 RF device ID's for the existing RF remote controls the plugin is listning to to update the state of the fan.
You are able to capture the id of you RF remote, by setting the log settings to 3 in the advanced settings menu. After pressing a button, you will see the ID of the RF.

In case a timerfunction is called (timer 1..3), a internal timer is running as estimate for the elapsed time.

For the lazy people or people with no codeskills a binary is added for ESPEASY with all stable plugins including this plugin
