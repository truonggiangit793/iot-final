#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define WEB_SERVER_PORT 80
#define BAUD_RATE 115200

ESP8266WebServer webServer(WEB_SERVER_PORT);
WiFiClient client;
HTTPClient http;

String arduinoData = "0";
const char * SSID = "Hog1_2.4G";
const char * PSW = "0903701132";
const char * NODEJS_SERVER = "192.168.1.25";
const int NODEJS_PORT = 3000;

void wifi_connection(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PSW);
  Serial.begin(BAUD_RATE);
  Serial.println("Wait for connecting to wifi...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Connecting to ");
    Serial.print(SSID);
    Serial.println("...");
  };
  Serial.println("Wifi connected successfully!");
  Serial.print("ESP Local IP address: ");
  Serial.println(WiFi.localIP());
}

void wifi_listening_connection(){
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Diconnected to ");
    Serial.print(SSID);
    Serial.println("...");
  };
}

void initialWebServer(){
  webServer.on("/", [](){
    webServer.send(200, "text/plain", "Hello world!");
  });
  webServer.begin();
}

void setup() {
  wifi_connection();
  initialWebServer();
}

void sendDataToServer(){
  client.print(
    String("GET /api/v1/new-data/")+ 
    String(arduinoData) + " HTTP/1.1\r\n" +
    "Host: " + NODEJS_SERVER + "\r\n" +
    "Connection: close\r\n\r\n"
  );
}

void readDataSerial(){
  if (Serial.available()) {
     arduinoData = Serial.readString();
     Serial.println(arduinoData);
     sendDataToServer();
  };
}

void NODEJS_listening(){
  if (!client.connect(NODEJS_SERVER, NODEJS_PORT)) {
    Serial.println("Connection to NodeJS server failed!");
    return;
  };
}

void loop() {
  WiFiClient client;
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  };
  webServer.handleClient();
  wifi_listening_connection();
  NODEJS_listening();
  readDataSerial();
}


