#include "head.h"

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.printf("[IoT Server] Disconnected!\n");
			break;
		case WStype_CONNECTED: {
			//Serial.printf("[WSc] Connected to url: %s\n", payload);

			DynamicJsonBuffer jsonBuffer;
  		  	JsonObject& root = jsonBuffer.createObject();

  		  	root["device"] = "Genie";
  		  	root["chipID"] = ESP.getChipId();
  		  	root["user"] = user;
  		  	root["pass"] = pass;
			root["message"] = "Genie-" + String(ESP.getChipId()) + " CONNECTED!";
  		  	char jsonString[100];
  		  	root.printTo(jsonString);
			webSocket.sendTXT(jsonString);	// send message to server when Connected
		}
			break;
		case WStype_TEXT:
		{
			DynamicJsonBuffer jsonBuffer;
			JsonObject& json = jsonBuffer.parseObject((char*)payload);

			if (json.success()) {

				char jsonToSend[200];
                json["message"].printTo(jsonToSend);
				JsonObject& root = json["message"];
				if (root.success()) {
					
					if( root["rtss"]["id"].success() && root["rtss"]["enabled"].success() && root["rtss"]["isOn"].success()){
						id = root["rtss"]["id"];
						enabled[id] = root["rtss"]["enabled"];
						isOn[id] = root["rtss"]["isOn"];
					}
					if( root["rtss"]["id"].success() && root["rtss"]["phaseControl"].success() && root["rtss"]["power"].success()){
						id = root["rtss"]["id"];
						isActivated[id] = root["rtss"]["phaseControl"];
						power[id] = root["rtss"]["power"];
					}
					if( root["options"].success()){
						searchLight = root["options"]["searchLight"];
						keepLEDon = root["options"]["keepLEDOn"];
						reset = root["options"]["reset"];
						blinkTime = root["options"]["blinkTime"];
					}
				}
				Serial.println(jsonToSend);
			} else {
				Serial.println("failed to load json config");
			}
			//Serial.printf("%s\n", payload);
		// send message to server
			break;
		}
		case WStype_BIN:
			Serial.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);
			// webSocket.sendBIN(payload, length);		// send data to server
			break;
		default:
			break;
	}
}

void websocketsStarter(){
	webSocket.begin(server, wsport, url); 	// server address, wsport and URL
	webSocket.onEvent(webSocketEvent);
	webSocket.setReconnectInterval(5000);
}
