#include <ESP8266WiFi.h>
#include <DHT.h>


const char* ssid = "YOURSSID";
const char* password = "YOURPASSWORD";


WiFiServer server(80);


String header;

String LED_PINState = "off";
String LED_PIN2State = "off";
String output3State = "off";


//PINS
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define LED_PIN D2
#define LED_PIN2 D3
#define LED_RED D10
#define LED_GREEN D9
#define LED_BLUE D8

//Rainbow LED
int red = 254;
int green = 1;
int blue = 127;
int red_direction = -1;
int green_direction = 1;
int blue_direction = -1;
void setColor(int red, int green, int blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}
unsigned long currentTime = millis();

unsigned long previousTime = 0;

const long timeoutTime = 2000;



void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);



  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {


            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            IPAddress clientIP = client.remoteIP();


            if (header.indexOf("GET /1/on") >= 0) {
              LED_PINState = "on";
              digitalWrite(LED_PIN, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              LED_PINState = "off";
              digitalWrite(LED_PIN, LOW);
            } else if (header.indexOf("GET /2/on") >= 0) {
              LED_PIN2State = "on";
              digitalWrite(LED_PIN2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              LED_PIN2State = "off";
              digitalWrite(LED_PIN2, LOW);
            } else if (header.indexOf("GET /3/on") >= 0) {
              output3State = "on";
              digitalWrite(LED_GREEN, HIGH);
              digitalWrite(LED_RED, HIGH);
              digitalWrite(LED_BLUE, HIGH);
              red = 254;
              green = 1;
              blue = 127;
              red_direction = -1;
              green_direction = 1;
              blue_direction = -1;
            } else if (header.indexOf("GET /3/off") >= 0) {
              output3State = "off";
              digitalWrite(LED_GREEN, LOW);
              digitalWrite(LED_RED, LOW);
              digitalWrite(LED_BLUE, LOW);
              red = 0;
              green = 0;
              blue = 0;
              red_direction = 0;
              green_direction = 0;
              blue_direction = 0;
            }

            //HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<html lang=\"pl\">");
            client.println("<head><meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>Arduino Led</title>");
            client.println("<style>html { background-color:#d9d9d9; font-family: system-ui; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("h1, h2, h3{font-size: 24px;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;border-radius: 12px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;border-radius: 12px;}</style>");
            client.println("<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\"></head>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js\"></script>");


            client.println("<body><h1>Arduino LED Serwer</h1>");
            client.println("<h2>IP Adress: " + clientIP.toString() + "</h2>");
            client.println("<h3><i class=\"fas fa-thermometer-half\"></i> Temperatura: " + String(temperature) + " &#8451;</h3>");
            client.println("<h3><i class=\"fas fa-tint\"></i> Wilgotność: " + String(humidity) + " %</h3>");



            client.println("<p>On/Off LED 1 - Status " + LED_PINState + "</p>");

            if (LED_PINState == "off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            client.println("<p>On/Off LED 2 - Status " + LED_PIN2State + "</p>");

            if (LED_PIN2State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>On/Off LED 3 - Status " + output3State + "</p>");

            if (output3State == "off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<script>$(document).ready(function(){");
            client.println("setInterval(function(){");
            client.println("$.ajax({url: \"/\", success: function(result){");
            client.println("$(\"html\").html(result);");
            client.println("}});}, 10000);});</script>");
            client.println("</body></html>");


            client.println();

            break;
          } else {  //
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }


    header = "";

    client.stop();
    Serial.println("Disconected.");
    Serial.println("");
  }
  red = red + red_direction;
  green = green + green_direction;
  blue = blue + blue_direction;

  if (red >= 255 || red <= 0) {
    red_direction = red_direction * -1;
  }
  if (green >= 255 || green <= 0) {
    green_direction = green_direction * -1;
  }
  if (blue >= 255 || blue <= 0) {
    blue_direction = blue_direction * -1;
  }
  setColor(red, green, blue);
  delay(5);
}
