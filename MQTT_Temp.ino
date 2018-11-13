

#include <OneWire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

float t, h, p;
const char* ssid = "JumperOK";
const char* password = "specialized200926";
const char* mqttServer = "109.86.162.193";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);	
Adafruit_BME280 bme;
Adafruit_BMP280 bmp;
void setup()
{
	Wire.begin();
	Wire.pins(D2, D1);
	Serial.begin(9600);
	
	Serial.println("Testing BME280 sensor");

	if (bme.begin()) {
		Serial.println("Could not find BME280 sensor");
		while (1);
	}
	Serial.println("BMP280 OK");
	
	Serial.println("Testing BMP280 sensor");
	if (bmp.begin(0x77)) {
		Serial.println("Could not find BMP280 sensor");
		while (1);
	}
	Serial.println("BMP280 OK");
	
		WiFi.begin(ssid, password);
	Serial.print("-=Connecting to WiFi=-");

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}

	Serial.println("WiFi connected sucesesfuly");
	delay(1000);
	client.setServer(mqttServer, mqttPort);
	//client.setCallback(callback);

	while (!client.connected()) {
		Serial.println("Connecting to MQTT...");

		if (client.connect("ESP8266Client")) {
			Serial.println("Connected to MQTT server 109.86.162.193:1883");
		}
	}
	

}

void loop()
{		
	t = bme.readTemperature();
	Serial.println(bmp.readTemperature());
	client.loop();
	client.publish("esp/sensors/temperature","t");
	
	
	client.publish("esp/sensors/humidity", "h");
	Serial.println(bme.readHumidity());
	
	//p = bme.readPressure();
	client.publish("esp/sensors/pressure", "p");
	Serial.println(bmp.readPressure());
	delay(5000);
}
