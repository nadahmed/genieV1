#include "head.h"

uint8_t relay[4] = {D5,D6,D1,D2};
size_t sizeOfRtss = 4;
uint8_t wifi_search = 16;//D0;
uint8_t id;
uint8_t power[4];

bool enabled[4];
bool isOn[4];
bool isActivated[4] = {0,0,0,0};
bool searchLight;
bool keepLEDon=true;
bool reset;
bool swState[4];
uint32_t blinkTime = 0;

void pinInitialization(){
    for(uint8_t i = 0; i<sizeOfRtss ; i++){
        pinMode(relay[i],OUTPUT);
        digitalWrite(relay[i], HIGH);
        swState[i] =false;
        enabled[i]=false;
        isOn[i]=true;
        isActivated[i]=false;

    }
    pinMode(wifi_search,OUTPUT);

    searchLight= true;
    keepLEDon=true;
    reset=false;
    blinkTime = 0;
}

void operate(){

    for(uint8_t i = 0; i<sizeOfRtss ; i++){
        if(enabled[i]){
            digitalWrite(relay[i], !isOn[i]);
        }
        else{
            digitalWrite(relay[i], HIGH);
        }

    }
}

void printValues(){
    Serial.println("");
    for (unsigned int i = 0 ; i< sizeOfRtss ; i++){

        Serial.print(enabled[i]);
        Serial.print('\t');
        Serial.print(isOn[i]);
        Serial.print('\t');
        Serial.print(isActivated[i]);
        Serial.print('\t');
        Serial.println(power[i]);

    }

    Serial.println(searchLight);
    Serial.println(keepLEDon);
    Serial.println(reset);
    Serial.println('\n');

}
