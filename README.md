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

![protophoto](https://lh3.googleusercontent.com/GRt9LnZoD8rHFXhTz15vf2pU71XCoh17dhCFR_NNDWe_8UAhNNGC6w-yoQUkA5FIlvjP8FSWcJau4aLtk04Bs3XF24WykdSkymKaI-kk7AXAiEfKYp3DKX8juWpzt5tSr2-nxTxo8yj2y5dWMSNFs2WC6KdMtKPqSim1B8VwsLoXIFuTZehj-FwhOSfe7tYHyRPjyLr7K_XaQ9hUGWMVJ9ySXoz3YH3nqpHu4_9kOOf2mc4BZuYftLI8je9SbTuH3bbjKCtDaVr5ZBPdirQ3lXFxcJ0FPVpRu8bNddV-nE1F2FNSUWtDa7CYMlZ7KNXSlXeVi4zZ3RCQo5Pcb-GMeCOJTFtw2X3OQ8iMUzMLC3CkvUXUhgHyKlzZz9yH6aze0C8mXLESEx7qwaCj265nv2zIQFrr-1RlJb4km6VhrxNigRsB3MzPypOc2Cu1cWHhdml79479NxkqIZOXx5dzhIw6SMLqT72NLI1RWcWdNpweo-arTpoOx3aFRCZI8RmGgNobRJtRoo7NvrpisnnN0riKNBBP3chsfaCqG-S3jZYrPRXxYGxBF83ezpbC3c1OI7YXd5veIyYC2FX4o1WEnincUPRQNQQVHqnspQhj1wc=w1350-h807-no)

[ArduinoJson]:https://github.com/bblanchon/ArduinoJson
[esp8266-oled-ssd1306]:https://github.com/squix78/esp8266-oled-ssd1306
[brzo_i2c]:https://github.com/pasko-zh/brzo_i2c
[timelib]:https://github.com/PaulStoffregen/Time
