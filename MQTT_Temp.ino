

#include <SparkFunBME280.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//u_char MQTT_resp;
int sleepTimeS;
unsigned long timer; //for time delay
char res[8]; //bufer for convert float to string
float temper, hum, press;
const char* ssid = "Edimax_2g";
const char* password = "magnaTpwd0713key";
const char* mqttServer = "109.86.162.193";
const int mqttPort = 1883;


WiFiClient espClient;
PubSubClient client(espClient);	
BME280 mySensor;

void callback(char* topic, byte* payload, unsigned int length);


void setup()
{
	Wire.begin();
	Wire.pins(D2, D1);
	Serial.begin(9600);
	
	Serial.println("Testing BME280 sensor");

	
	mySensor.settings.commInterface = I2C_MODE;
	mySensor.settings.I2CAddress = 0x76;
	if (mySensor.beginI2C() == false) //Begin communication over I2C
	{
		Serial.println("The sensor did not respond. Please check wiring.");
		while (1); //Freeze
	}
	Serial.println("BMP280 OK");
	
	WiFi.begin(ssid, password);
	
	Serial.println("Connecting to WiFi");

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}

	Serial.println("WiFi connected sucesesfuly");
	delay(1000);
	
	client.setServer(mqttServer, mqttPort);
	client.setCallback(callback);

	while (!client.connected()) {
		Serial.println("Connecting to MQTT...");

		if (client.connect("ESP8266Client")) {
			Serial.println("Connected to MQTT server 109.86.162.193:1883");
		}
	}
	

}
void callback(char* topic, byte* message, unsigned int length) {
	Serial.print("Message arrived on topic: ");
	Serial.print(topic);
	Serial.print(". Message: ");
	String messageTemp;

	for (int i = 0; i < length; i++) {
		Serial.print((char)message[i]);
		messageTemp += (char)message[i];
	}
	Serial.println();


	if (String(topic) == "esp/buttons") {
		
		if (messageTemp == "on") {
			Serial.println("on");

		}
		else if (messageTemp == "off") {
			Serial.println("off");

		}
	}
}

void loop()
{
			
		client.loop();
		
		
		if (millis() > (timer + 5000)) {
			timer = millis();
			
			client.subscribe("esp/buttons");
			
			temper = mySensor.readTempC(), 1;
			//Serial.println(temper);
			dtostrf(temper, 4, 2, res);
			client.publish("esp/sensors/temperature", res);

			hum = mySensor.readFloatHumidity(), 1;
			//Serial.println(hum);
			dtostrf(hum, 4, 2, res);
			client.publish("esp/sensors/humidity", res);

			press = mySensor.readFloatPressure();
			press = press / 133.322;
			//Serial.println(press);
			dtostrf(press, 4, 1, res);
			client.publish("esp/sensors/pressure", res);

			String json = "{\"data\":{" "\"temperature\": \"" + String(temper) + "\"," "\"humidity\": \"" + String(hum) + "\"," "\"pressure\": \"" + String(press) + "\"}" "}";
			// Convert JSON string to character array
			char jsonChar[100];
			json.toCharArray(jsonChar, json.length() + 1);

			// Publish JSON character array to MQTT topic
			client.publish("esp/sensors/json", jsonChar);

			Serial.println("");
			Serial.println("JSON data format");
			Serial.println(json);
		}
				
}
