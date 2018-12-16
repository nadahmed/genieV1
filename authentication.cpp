/**
   reuseConnection.ino

    Created on: 22.11.2015

*/

#include <head.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

HTTPClient http;
String csrfToken;
const char *headerkeys[] = {"Set-Cookie", "Cookie"};

size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);

String PostData = "username=noorz@gmail.com&password=noor9479";
String getValue(String data, const char separator, int index);

int httpCode, postCode;

void authenticate()
{
    // allow reuse (if server supports it)
    http.setReuse(true);

    static bool loginFlag = false;
    // wait for WiFi connection

    while (!loginFlag)
    {
        if (csrfToken == "")
        {
            http.begin("http://192.168.2.191/login/");
            http.collectHeaders(headerkeys, headerkeyssize);
            httpCode = http.GET();

            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            csrfToken = getValue(getValue(http.header("Set-Cookie"), ';', 0), '=', 1);
            http.end();
            delay(1000);
        }
        else if (postCode != 302)
        {

            http.begin("http://192.168.2.191/login/");
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            http.addHeader("cookie", "csrftoken=" + csrfToken);
            //String myPostData = "csrfmiddlewaretoken=" + csrfToken + "&" + PostData;
            http.collectHeaders(headerkeys, headerkeyssize);
            postCode = http.POST("csrfmiddlewaretoken=" + csrfToken + "&" + PostData);
            if (postCode == 302)
                loginFlag = true;
            USE_SERIAL.println(http.header("Set-Cookie"));
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", postCode);
            http.end();
        }
    }
/*
    if (postCode == 302 || loginFlag == true)
    {
        http.begin("http://192.168.2.191/");
        //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("cookie", "csrftoken=" + csrfToken);
        httpCode = http.GET();
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            http.writeToStream(&USE_SERIAL);
            http.end();
        }
    }
    //Serial.println("csrfmiddlewaretoken=" + csrfToken + "&" + PostData);

    delay(1000);
}
*/
String getValue(String data, const char separator, int index)
{

    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
