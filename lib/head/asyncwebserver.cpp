#include <head.h>
bool shouldReboot = false;
const char console[] PROGMEM = "<!DOCTYPE html><html><head> <meta charset=\"utf-8\"/> <title>Console for ESP</title> <style> textarea { resize: none; width:800px; background-color: #2e3b4c; color:#EFEF10; } .grid-container { display: grid; grid-template-columns: 80% 0px 10% 10%; grid-gap: 1px; padding: 1px; width: 800px; } </style></head><body> <textarea readonly id=\"console-log\" cols=\"100\" rows=\"20\" charswidth=\"23\"></textarea><br/> <div class=\"grid-container\"> <input id=\"console-message-input\" width = \"200px\" type=\"text\"/><br/> <input id=\"console-message-submit\" type=\"button\" value=\"Send\"/> <input id=\"console-clear-message\" type=\"button\" value=\"Clear\"/> </div> <input id=\"switch-0\" type=\"button\" value=\"Switch 1\"/> <input id=\"switch-1\" type=\"button\" value=\"Switch 2\"/> <input id=\"switch-2\" type=\"button\" value=\"Switch 3\"/> <input id=\"switch-3\" type=\"button\" value=\"Switch 4\"/></body><script>onload=document.getElementById(\"console-message-input\").value=\"\",state=[!0,!0,!0,!0];var consoleSocket=new WebSocket(\"ws://\"+window.location.host+\"/ws\");consoleSocket.onmessage=function(e){var o=JSON.parse(e.data),s=o.message.rtss;state[s.id]=!s.isOn;var n=JSON.stringify(o.message),t=document.getElementById(\"console-log\");t.scrollTop=t.scrollHeight,document.querySelector(\"#console-log\").value+=n+\"\\n\"},consoleSocket.onclose=function(e){console.error(\"Console socket closed unexpectedly\")},document.querySelector(\"#console-message-input\").focus(),document.querySelector(\"#console-message-input\").onkeyup=function(e){13===e.keyCode&&document.querySelector(\"#console-message-submit\").click()},document.querySelector(\"#console-message-submit\").onclick=function(e){var o=document.querySelector(\"#console-message-input\"),s=o.value;consoleSocket.send(JSON.stringify({message:s})),o.value=\"\"},document.querySelector(\"#switch-0\").onclick=function(e){message={rtss:{id:0,enabled:!0,isOn:state[0],phaseControl:!1,power:100}},consoleSocket.send(JSON.stringify({message:message}))},document.querySelector(\"#switch-1\").onclick=function(e){message={rtss:{id:1,enabled:!0,isOn:state[1],phaseControl:!1,power:100}},consoleSocket.send(JSON.stringify({message:message}))},document.querySelector(\"#switch-2\").onclick=function(e){message={rtss:{id:2,enabled:!0,isOn:state[2],phaseControl:!1,power:100}},consoleSocket.send(JSON.stringify({message:message}))},document.querySelector(\"#switch-3\").onclick=function(e){message={rtss:{id:3,enabled:!0,isOn:state[3],phaseControl:!1,power:100}},consoleSocket.send(JSON.stringify({message:message}))},document.querySelector(\"#console-clear-message\").onclick=function(e){var o=document.querySelector(\"#console-message-input\");o.value;o.value=\"\",onload=document.getElementById(\"console-log\").value=\"\"};</script></html>";

void httpServiceStarter(){
	httpServer.on("/", HTTP_GET, handleRoot);
	httpServer.onNotFound(handleNotFound);

	httpServer.on("/console", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(200, "text/html", console);
	});

	httpServer.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
	});
	httpServer.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
      shouldReboot = !Update.hasError();
      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot?"OK - Restarting Genie.":"FAIL");
      response->addHeader("Connection", "close");
      request->send(response);
    },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
      if(!index){
        Serial.printf("Update Start: %s\n", filename.c_str());
        Update.runAsync(true);
        if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
          Update.printError(Serial);
        }
      }
      if(!Update.hasError()){
        if(Update.write(data, len) != len){
          Update.printError(Serial);
        }
      }
      if(final){
        if(Update.end(true)){
          Serial.printf("Update Success: %uB\n", index+len);
        } else {
          Update.printError(Serial);
        }
      }
    });

	httpServer.begin();
	//Serial.println("[Server] HTTP server started");
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "HomePage, Coming soon...! Your IP is: " + String(ipStr));
}

void handleNotFound(AsyncWebServerRequest *request) {
/*    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
*/
    request->send(404,"Not Found");
}
