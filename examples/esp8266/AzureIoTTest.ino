// Please use an Arduino IDE 1.6.8 (once released) or an hourly build from 1/21/2016 or later from
// https://www.arduino.cc/en/Main/Software
// Prior builds may experience pre-processor function prototype failures.

#include <ESP8266WiFi.h>
#include <time.h>
#include "simplesample_http.h"
#include "AzureIoT.h"


char ssid[] = "ssid"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

static const char* connectionString = "HostName=[host].azure-devices.net;DeviceId=[device];SharedAccessKey=[key]";

#define FAN_PIN 5

int status = WL_IDLE_STATUS;

// WORKAROUND ARDUINO PREPROCESSOR ISSUE
void initSerial();
void initWifi();
void initTime();
void initFan();
void initAzureIotHub();
void cleanupAzureIotHub();

// Define the Model
BEGIN_NAMESPACE(WeatherStation);

DECLARE_MODEL(ContosoAnemometer,
WITH_DATA(ascii_char_ptr, DeviceId),
WITH_DATA(int, WindSpeed),
WITH_ACTION(TurnFanOn),
WITH_ACTION(TurnFanOff),
WITH_ACTION(SetAirResistance, int, Position)
);

END_NAMESPACE(WeatherStation);

// Local instance of model
ContosoAnemometer* myWeather = NULL;

void setup() {
    initSerial();
    initWifi();
    initTime();

    initFan();

    initAzureIotHub();
}

void initAzureIotHub() {
    init_azureiot_hub(connectionString);
    Serial.print("Inited azure hub");
    myWeather = CREATE_MODEL_INSTANCE(WeatherStation, ContosoAnemometer);
    Serial.print("Created model");
    register_azureiot_model(myWeather);
    Serial.println("Registered model");
}

void cleanupAzureIotHub() {
    DESTROY_MODEL_INSTANCE(myWeather);
    cleanup_azureiot_hub();
}

void loop() {
    // Send one event before looping
    srand((unsigned int)time(NULL));
    int avgWindSpeed = 10.0;
    myWeather->DeviceId = "myFirstDevice";
    myWeather->WindSpeed = avgWindSpeed + (rand() % 4 + 2);
    unsigned char* event;
    size_t eventSize;
    Serial.println("Generated event");
    if (SERIALIZE(&event, &eventSize, myWeather->DeviceId, myWeather->WindSpeed) != IOT_AGENT_OK)
    {
        LogInfo("Failed to serialize\r\n");
    }
    else
    {
        Serial.println("Serialized event");
        send_event(event, eventSize);
        free(event);
    }
    Serial.println("Sent event");

    // Loop and wait to handle messages (via callbacks below)
    while(1) {
        azureiot_dowork();
        delay(100);
    }

    cleanupAzureIotHub();
}

void initSerial() {
	// Start serial and initialize stdout
    Serial.begin(115200);
    Serial.setDebugOutput(true);
}

void initWifi() {
	// Attempt to connect to Wifi network:
	Serial.print("Attempting to connect to SSID: ");
	Serial.println(ssid);

	// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
	status = WiFi.begin(ssid, pass);

	while (WiFi.status() != WL_CONNECTED) {
	    delay(500);
	    Serial.print(".");
	}
  
  	Serial.println("Connected to wifi");
}

void initTime() {
    time_t epochTime;

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true) {
        epochTime = time(NULL);

        if (epochTime == 0) {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        } else {
            Serial.print("Fetched NTP epoch time is: ");
            Serial.println(epochTime);
            break;
        }
    }
}

void initFan() {
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, HIGH);
}

EXECUTE_COMMAND_RESULT TurnFanOff(ContosoAnemometer* myWeather) {
    Serial.println("Turning fan Off");
    digitalWrite(FAN_PIN, HIGH);
    return EXECUTE_COMMAND_SUCCESS;
}

EXECUTE_COMMAND_RESULT TurnFanOn(ContosoAnemometer* myWeather) {
    Serial.println("Turning fan On");
    digitalWrite(FAN_PIN, LOW);
    return EXECUTE_COMMAND_SUCCESS;
}

EXECUTE_COMMAND_RESULT SetAirResistance(ContosoAnemometer*, int) {
    Serial.print("Setting Air Resistance");
    return EXECUTE_COMMAND_SUCCESS;
}