# espbtc
Coinbase bitcoin ticker using an ESP8266 and SSD1306 OLED

## Libraries used
- Arduino esp8266 [2.4.0-rc2](https://github.com/esp8266/Arduino/releases/tag/2.4.0-rc2)
- [ArduinoJson]
- [esp8266-oled-ssd1306]
- [brzo_i2c] (for faster screen interfacing)
- [timelib]

## Description
It gets NTP time, uses [coinbase's JSON API](https://developers.coinbase.com/api/v2#prices) 
by http GET'ing `https://api.coinbase.com/v2/prices/BTC-USD/buy`, parsing the json and displaying the 
latest USD price on the screen as well as a timestamp of the most recent successful query so that the user 
knows the data is not stale.

If the parsing fails or the connection doesn't complete (and it happens more often than you'd think)
it shows an error sign with accompanying error message, so that the wrong value doesn't mistakenly get used.
It will then retry the next time and the error usually goes away.

Initially started on Arduino IDE  but then moved to use platformio.

[ArduinoJson]:https://github.com/bblanchon/ArduinoJson
[esp8266-oled-ssd1306]:https://github.com/squix78/esp8266-oled-ssd1306
[brzo_i2c]:https://github.com/pasko-zh/brzo_i2c
[timelib]:https://github.com/PaulStoffregen/Time
