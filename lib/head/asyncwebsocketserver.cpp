#include "head.h"

// SKETCH BEGIN
AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        //Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());

        //    DynamicJsonBuffer jsonBuffer;
        //    JsonObject& root = jsonBuffer.createObject();

        //    root["device"] = "Genie";
        //    root["chipID"] = ESP.getChipId();
        //    root["user"] = user;
        //    root["pass"] = pass;
        //    root["message"] = "Genie-" + String(ESP.getChipId()) + " CONNECTED!";
        for (uint8_t i = 0; i < sizeOfRtss; i++)
        {

            char temp[200];
            char *jsonString = temp;
            jsonSerializer(i, jsonString);

            client->printf(jsonString);
        }

        //client->printf(jsonString.c_str());
        //client->ping();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        //Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        char msg[200] = "";
        if (info->final && info->index == 0 && info->len == len)
        {
            //the whole message is in a single frame and we got all of it's data
            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < info->len; i++)
                {
                    msg[i] += (char)data[i];
                }
            }
            else
            {
                char buff[3];
                for (size_t i = 0; i < info->len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    strcpy(msg, buff);
                }
            }
            //Serial.printf("%s\n",msg.c_str());

            if (info->opcode == WS_TEXT)
            {
                //client->text(msg.c_str());
                ws.textAll(msg);
            }
            else
                client->binary("{\"message\":\"I got your binary message\"}");

            const size_t bufferSize = 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5) + 80;
            StaticJsonBuffer<bufferSize> jsonBuffer;
            JsonObject &json = jsonBuffer.parseObject(msg);

            if (json.success())
            {

                JsonObject &root = json["message"];
                if (root.success())
                {

                    if (root["rtss"]["id"].success() && root["rtss"]["enabled"].success() && root["rtss"]["isOn"].success())
                    {
                        id = root["rtss"]["id"];
                        enabled[id] = root["rtss"]["enabled"];
                        isOn[id] = root["rtss"]["isOn"];
                    }
                    if (root["rtss"]["id"].success() && root["rtss"]["phaseControl"].success() && root["rtss"]["power"].success())
                    {
                        id = root["rtss"]["id"];
                        isActivated[id] = root["rtss"]["phaseControl"];
                        power[id] = root["rtss"]["power"];
                    }
                    if (root["options"].success())
                    {
                        searchLight = root["options"]["searchLight"];
                        keepLEDon = root["options"]["keepLEDOn"];
                        reset = root["options"]["reset"];
                        blinkTime = root["options"]["blinkTime"];
                    }
                }
            }
            else
            {
                Serial.println("failed to load json config");
            }
        }
    }
}

void websocketServerStarter()
{
    //ws.setAuthentication(user.c_str(), pass.c_str());
    ws.onEvent(onWsEvent);
    httpServer.addHandler(&ws);
}
