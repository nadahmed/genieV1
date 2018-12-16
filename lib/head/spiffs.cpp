#include "head.h"

void spiffs(){
	//SPIFFS.format();		//clean FS, for testing
	//Serial.println("mounting FS...");	//read configuration from FS json
	if (SPIFFS.begin()) {
		//Serial.println("mounted file system");
		if (SPIFFS.exists("/config.json")) {
			//Serial.println("reading config file");
			File configFile = SPIFFS.open("/config.json", "r");		//file exists, reading and loading
			if (configFile) {
				//Serial.println("opened config file");
				size_t size = configFile.size();
				std::unique_ptr<char[]> buf(new char[size]);		// Allocate a buffer to store contents of the file.
				configFile.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				json.printTo(Serial);
				Serial.println("");

				if (json.success()) {
					//Serial.println("\nparsed json");
					wsport = json["wsport"];
					strcpy(server, json["server"]);
					strcpy(url, json["URL"]);
					strcpy(user, json["user"]);
					strcpy(pass, json["pass"]);
				}
				else {
					configFile.close();
					Serial.println("Failed to parse config file");
					SPIFFS.remove("/config.json");
					ESP.restart();
					delay(5000);
				}
				configFile.close();
			}
		}
		else {
			DynamicJsonBuffer jsonBuffer;
			JsonObject& json = jsonBuffer.createObject();
			json["user"] = user;
			json["pass"] = pass;
			json["server"] = server;
			json["URL"] = url;
			json["wsport"] = wsport;
			File configFile = SPIFFS.open("/config.json", "w");
			if (!configFile) {
				Serial.println("failed to open config file for writing");
			}
			else{
				json.printTo(Serial);
				Serial.println("");
				json.printTo(configFile);
				configFile.close();
			}
		}
	}
	else {
		Serial.println("failed to mount FS");
	}
}