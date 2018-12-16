#include <head.h>

#define DRD_TIMEOUT 1
#define DRD_ADDRESS 0		// RTC Memory Address for the DoubleResetDetector to use

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
bool resetFlag = false;
bool useLocally = true;

int wsport = 8266;
char server[17] = "192.168.2.191";
char url[30] = "/ws/console/";
char user[30] = "Username";
char pass[30] = "Password";

void setup() {
	Serial.begin(38400);
	Serial.setDebugOutput(false);
	Serial.println();
	Serial.println();
	Serial.println();

	if (drd.detectDoubleReset()) {
        Serial.println("Double Reset Detected");
        resetFlag = true;
    } else {
        resetFlag = false;
    }


	for(uint8_t t = 4; t > 0; t--) {
		//Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(500);
	}

	pinInitialization();

	drd.stop();

	spiffs();
	wifiConnection();
    websocketServerStarter();
	httpServiceStarter();
	mdnsStarter();
	websocketsStarter();
}

unsigned long pre2 = millis();
bool noWifiFlag =true;

void loop() {
	if (shouldReboot){		//If webupdate is Successful then reboot device
		delay(2000);
		ESP.restart();
		delay(5000);
	}
	if (WiFi.status() != WL_CONNECTED) {
		if(noWifiFlag){
			ticker.attach(0.05, tick);
			noWifiFlag = false;
			pre2 = millis();
		}

		if (millis()-pre2 > (120 * 1000)){
			ticker.detach();
			digitalWrite(wifi_search, HIGH);
		}
		
		WiFi.reconnect();
	} else {
		if(!noWifiFlag){
			ticker.detach();
		    digitalWrite(wifi_search, LOW);
		}
		noWifiFlag = true;
	}
	operate();
}
