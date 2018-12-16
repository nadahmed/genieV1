#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "ESPAsyncDNSServer.h"
#include <ESP8266mDNS.h>
#include <ESPAsyncWifiManager.h>
#include <WebSocketsClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <DoubleResetDetector.h>
#include <Ticker.h>

extern int wsport;
extern char server[17];
extern char url[30];
extern char user[30];
extern char pass[30];
extern bool resetFlag;
extern AsyncWebServer httpServer;
extern WebSocketsClient webSocket;
extern AsyncWebSocket ws;
extern bool useLocally;
extern DoubleResetDetector drd;
extern Ticker ticker;
extern char ipStr[15];
extern bool shouldReboot;

extern uint8_t relay[4];
extern size_t sizeOfRtss;
extern uint8_t wifi_search;//D0;

extern uint8_t id;
extern uint8_t power[4];
extern bool enabled[4];
extern bool isOn[4];
extern bool isActivated[4];
extern bool searchLight;
extern bool keepLEDon;
extern bool reset;
extern bool swState[4];
extern uint32_t blinkTime;

void tick();
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
void websocketsStarter();
void wifiConnection();
void configModeCallback (AsyncWiFiManager *myWiFiManager);
void saveConfigCallback ();
void spiffs();
void mdnsStarter();
void handleSwitchOn(AsyncWebServerRequest *request);
void handleSwitchOff(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
void httpServiceStarter();
void accessPoint(AsyncWiFiManager wifiManager);
void websocketServerStarter();
void operate();
void pinInitialization();
void printValues();
void jsonSerializer(uint8_t i, char*& myString);
//void jsonParser(const char* json);