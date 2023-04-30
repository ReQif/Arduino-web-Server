# Arduino-web-Server
This code is an Arduino sketch that uses an ESP8266 WiFi module and a DHT11 temperature and humidity sensor to control several LEDs. It creates a web server on port 80 that listens for incoming HTTP requests, and depending on the URL parameters, it turns on or off different LEDs. There are three LEDs controlled: LED_PIN, LED_PIN2, and a RGB LED that is controlled by setting the red, green, and blue components.

The web server generates a basic HTML page that displays the current state of the LEDs and provides buttons to turn them on and off. The page is served over the WiFi network and can be accessed by any device that is connected to the same network as the ESP8266 module.

The DHT11 sensor is used to read the current temperature and humidity values, but this data is not displayed on the web page. Instead, the values are stored in the "temperature" and "humidity" variables, which can be used for other purposes in the code.
