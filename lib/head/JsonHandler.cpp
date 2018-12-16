#include "head.h"
/*
void jsonParser(const char* jsonToParse, char*& serialized)
{
    //Serial.println(json);
    const size_t bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5) + 100;
    StaticJsonBuffer<bufferSize> jsonBuffer;

    JsonObject &root = jsonBuffer.parseObject(jsonToParse);
    if (root.success())
    {

        if (root["rtss"].success())
        {
            int8_t id = -1;
            if (root["rtss"]["id"].success())
                id = root["rtss"]["id"];
            if (id >= 0 && id <= 3)
            {
                if (root["rtss"]["enabled"].success())
                    enabled[id] = root["rtss"]["enabled"];
                if (root["rtss"]["isOn"].success())
                    isOn[id] = root["rtss"]["isOn"];
                if (root["rtss"]["phaseControl"].success())
                    isActivated[id] = root["rtss"]["phaseControl"];
                if (root["rtss"]["id"].success())
                    power[id] = root["rtss"]["power"];

                jsonSerializer(id, serialized);
                //saveStates(id);
            }
        }
        if (root["options"].success())
        {
            if (root["options"]["searchLight"].success())
                searchLight = root["options"]["searchLight"];
            if (root["options"]["keepLEDOn"].success())
                keepLEDon = root["options"]["keepLEDOn"];
            if (root["options"]["reset"].success())
                reset = root["options"]["reset"];
            if (root["options"]["blinkTime"].success())
                blinkTime = root["options"]["blinkTime"];
        }

    }

    else
        Serial.println("[JSON] parse failed");
}*/

void jsonSerializer(uint8_t i, char*& myString)
{
    const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5);
    StaticJsonBuffer<bufferSize> jsonBuffer;
    JsonObject &mess = jsonBuffer.createObject();
    JsonObject &root = mess.createNestedObject("message");
    JsonObject &rtssJson = root.createNestedObject("rtss");

    rtssJson["id"] = i;
    rtssJson["enabled"] = enabled[i];
    rtssJson["isOn"] = !(bool)digitalRead(relay[i]) ;
    rtssJson["power"] = power[i];
    rtssJson["phaseControl"] = isActivated[i];
    
    char jsonString[200];
    mess.printTo(jsonString);
    strcpy(myString,jsonString);
}