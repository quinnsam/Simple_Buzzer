/*
 * NodeMCU/ESP8266 act as AP (Access Point) and simplest Web Server
 * to control GPIO (on-board LED)
 * Connect to AP "arduino-er", password = "password"
 * Open browser, visit 192.168.4.1
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "arduino-er";
const char *password = "password";
int stateLED = LOW;
int buzzPin = D2;

ESP8266WebServer server(80);

void handleRoot() {
    response();
}

void buzz() {
  for (int i=0; i < 5; i++) {
    digitalWrite(buzzPin, HIGH);
    delay(300);
    digitalWrite(buzzPin, LOW);
    delay(100);
  }
  response();
}

void handleLedOn() {
  stateLED = LOW;
  digitalWrite(LED_BUILTIN, stateLED);
  response();
}

void handleLedOff() {
  stateLED = HIGH;
  digitalWrite(LED_BUILTIN, stateLED);
  buzz();
  response();
}

const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle = "<h1>Dog Buzzer</h1><br/>\n";
const String HtmlLedStateLow = "<big>LED is now <b>ON</b></big><br/>\n";
const String HtmlLedStateHigh = "<big>LED is now <b>OFF</b></big><br/>\n";
const String HtmlButtons = 
    "<a href=\"LEDOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"LEDOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>";
const String HtmlButton = "<a href=\"BuzzOn\"><button style=\"display: block; width: 100%;\">Buzz</button></a><br/>";
    
void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
//  if(stateLED == LOW){
//    htmlRes += HtmlLedStateLow;
//  }else{
//    htmlRes += HtmlLedStateHigh;
//  }

  htmlRes += HtmlButton;
  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

void setup() {
    delay(1000);
    Serial.begin(9600);
    Serial.println();

    WiFi.softAP(ssid, password);

    IPAddress apip = WiFi.softAPIP();
    Serial.print("visit: \n");
    Serial.println(apip);
    server.on("/", handleRoot);
    server.on("/LEDOn", handleLedOn);
    server.on("/LEDOff", handleLedOff);
    server.on("/BuzzOn", buzz);
    server.begin();
    Serial.println("HTTP server beginned");
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(buzzPin, OUTPUT);
    digitalWrite(LED_BUILTIN, stateLED);
    digitalWrite(buzzPin, LOW);
}

void loop() {
    server.handleClient();
}
