#include <head.h>

Ticker ticker;
AsyncDNSServer dns;
AsyncWebServer httpServer(wsport);

char ipStr[15];

void tick()
{
    //toggle state
    int state = digitalRead(wifi_search); // get the current state of GPIO1 pin
    digitalWrite(wifi_search, !state);    // set pin to the opposite state
}

bool shouldSaveConfig = false;

void saveConfigCallback()
{
    shouldSaveConfig = true;
}

void wifiConnection()
{

    if (resetFlag || WiFi.SSID().equals(""))
    {
        ticker.attach(0.2, tick);
        Serial.println("Begin Smart Config");
        WiFi.beginSmartConfig();
        while (!WiFi.smartConfigDone())
        {
            Serial.print(".");
            delay(500);
        }
        Serial.println("Done.");
        resetFlag = false;
    }
    if (!resetFlag)
    {

        Serial.println("{\"options\" : {\"searchLight\" : true, \"keepLEDOn\" : true, \"reset\": false, \"blinkTime\": 100 }}");
        WiFi.mode(WIFI_STA);
        WiFi.begin();
        ticker.attach(0.05, tick);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
        }

        /*
		wifiManager.setConfigPortalTimeout(1);
        ticker.attach(0.05, tick);
        drd.stop();
		if (!wifiManager.autoConnect("Genie", "pass1234")) {
			delay(3000);
			ESP.restart();
			delay(5000);
		}
		*/
    }

    ticker.detach();
    digitalWrite(wifi_search, LOW);

    strcpy(ipStr, WiFi.localIP().toString().c_str());
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());

    if (shouldSaveConfig)
    {

        //Serial.println("saving config");
        const size_t bufferSize = JSON_OBJECT_SIZE(5) + 90;
        StaticJsonBuffer<bufferSize> jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
        json["user"] = user;
        json["pass"] = pass;
        json["server"] = server;
        json["URL"] = url;
        json["wsport"] = wsport;
        if (SPIFFS.begin())
        {
            SPIFFS.remove("/config.json");
            File configFile = SPIFFS.open("/config.json", "w");
            if (!configFile)
            {
                Serial.println("failed to open config file for writing");
            }
            else
            {
                json.printTo(Serial);
                Serial.println("");
                json.printTo(configFile);
                configFile.close();
            }
        }
    }
}

void mdnsStarter()
{
    char hostString[16] = {0};
    sprintf(hostString, "GBV1_%06X", ESP.getChipId());
    if (!MDNS.begin(hostString))
    {
        //Serial.println("Error setting up MDNS responder!");
    }
    else
    {
        //Serial.println("mDNS responder started");
        MDNS.addService("genie", "tcp", 8267); // Add service to MDNS-SD
    }
}
