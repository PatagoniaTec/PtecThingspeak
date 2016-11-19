//Librerias Necesarias.
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "ThingSpeak.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>


//Defines
#define Debug

#define DHT11Pin 14
#define DHT22Pin 12
#define DHT11Type DHT11
#define DHT22Type DHT22


//Variables para ThingSpeak
unsigned long TSChannel = 22768;
const char * APIkey = "BK23DP4M0SSRJ25C";

//constructores
DHT dht11(DHT11Pin, DHT11Type);
DHT dht22(DHT22Pin, DHT22Type);
Adafruit_BMP280 bme;
WiFiClient client;

//Variables
float dht11T;
float dht11H;
float dht22T;
float dht22H;
float bmeT;
float bmeP;


void LeerSensores() {
  dht11T = dht11.readTemperature();
  dht11H = dht11.readHumidity();
  dht22T = dht22.readTemperature();
  dht22H = dht22.readHumidity();
  bmeT = bme.readTemperature();
  bmeP = bme.readPressure();
#ifdef Debug
  Serial.println(dht11T);
  Serial.println(dht11H);
  Serial.println(dht22T);
  Serial.println(dht22H);
  Serial.println(bmeT);
  Serial.println(bmeP);
#endif
}


void setup() {
  Wire.begin(); //GPIO2, GPIOO -> D4, D3
  //Wire.setClock(400000);
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
#ifdef Debug
  Serial.println("connected...yeey :)");
#endif
  ThingSpeak.begin(client);
  dht11.begin();
  dht22.begin();

#ifdef Debug
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    //while (1);
  }
#else
  bme.begin();
#endif
  delay(2000);
}

void loop() {
  LeerSensores();
  ThingSpeak.setField(1,dht11T); // dht11t
  ThingSpeak.setField(2,dht11H); // dht11h
  ThingSpeak.setField(3,bmeP); // bmeP
  ThingSpeak.setField(4,bmeT); // bmeT
  ThingSpeak.setField(5,dht22T); // dht22t
  ThingSpeak.setField(6,dht22H); // dht22h
  ThingSpeak.writeFields(TSChannel, APIkey);  
  delay(20000);
}
