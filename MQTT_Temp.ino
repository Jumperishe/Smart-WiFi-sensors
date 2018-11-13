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
#include <Adafruit_BMP280.h>
#include <PubSubClient.h>

const char* ssid = "Edimax_2g";
const char* password = "magnaTpwd0713key";
const char* mqttServer = "109.86.162.193";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);	

void setup()
{
	Serial.begin(9600);
	WiFi.begin(ssid, password);
	
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print("Connecting to WiFi...");
		delay(200);
	}

	Serial.print("WiFi connected sucesesfuly");
	Serial.println(WL_IDLE_STATUS);
						
	client.setServer(mqttServer, mqttPort);
	//client.setCallback(callback);

	while (!client.connected()) {
		Serial.println("Connected to MQTT");

		if (client.connect("ESP8266Client")) {
			Serial.println("Connected");
		}
	}
	

}

void loop()
{
	client.loop();
	client.publish("esp/test", "hello from esp8266");

	delay(500);
}
