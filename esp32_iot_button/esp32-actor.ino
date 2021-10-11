// Copyright (c) 2021 by Martin Willner <willner@dynamicprojection.com
// https://www.dynamicprojection.com

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

WiFiClient wificlient;

// SETUP START


// JCD is the MQTT-BROKER - remove this or comment out to use MDC-X as broker
#define JCD_IS_BROKER

#ifdef JCD_IS_BROKER
#define WLAN_SSID "JCD-7872034f" // JCD SSID
#define WLAN_PASS "77f60414" // JCD WLAN PASS - see JCD label!
#define IO_SERVER "192.168.1.1" // JCD WLAN IP 
#else
#define WLAN_SSID "MDC-X-HOTSPOT" // MDC-X SSID - factory default
#define WLAN_PASS "mdcmdc123" // MDC-X WLAN PASS - factory default
#define IO_SERVER "10.42.0.1" // MDC-X WLAN IP  - factory default
#endif

#define IO_SERVERPORT 1883 // DO NOT CHANGE FOR JCD
#define IO_USERNAME "" // DO NOT CHANGE FOR JCD
#define IO_KEY "" // DO NOT CHANGE FOR JCD

// ESP32 SETUP 
#define BUTTON_PIN 0  //ESP32 WROOM 32 breakout default; othr ESP32 use pin 3 - adjust to your breakout board! 
#define LED 2 //ESP32 WROOM 32 breakout breakout default  - adjust to your breakout board! 

Adafruit_MQTT_Client mqtt(&wificlient, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

// publish to ALL (global) subscribed mdcx systems
Adafruit_MQTT_Publish cmd = Adafruit_MQTT_Publish(&mqtt, "mdcx/global/osc"); //where to publish

// publish to a unique (uuid: 1234) subscribed mdcx system
// Adafruit_MQTT_Publish cmd = Adafruit_MQTT_Publish(&mqtt, "mdcx/1234/osc");

// A list of preset numbers to use as a "next" preset 
const int presetlist[]={1,2,4,6,8,10,23};

// SETUP END

volatile unsigned long DebounceTimer;
volatile int ButtonPressed;
volatile unsigned int delayTime = 100;
bool ledStatus = false;
unsigned long previousMillis = 0;
unsigned long interval = 30000;

void scan_wifi()
{
    int n = WiFi.scanNetworks();
    Serial.print(n);
    Serial.println(" WiFi-networks found!");
    if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println(WiFi.encryptionType(i));
            delay(10);
        }
    }
    Serial.println("");
}

void lighttoggle()
{
    ledStatus = !ledStatus;
    digitalWrite(LED, ledStatus);
    
}

void wificonnect()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        lighttoggle();
        delay(250);
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED, HIGH);
}

void MQTT_connect()
{
    int8_t ret;
    if (mqtt.connected())
    {
        return;
    }
    Serial.print("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0)
    { 
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection ...");
        mqtt.disconnect();
        delay(1000); // wait 5 seconds
    }
    Serial.println("MQTT Connected!");
}


void setup()
{
    Serial.begin(9600);
    Serial.println("HELLO");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED,OUTPUT);
    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN), []
        {
            if (ButtonPressed += (millis() - DebounceTimer) >= (delayTime))
                DebounceTimer = millis();
        },
        FALLING);
    scan_wifi();
    wificonnect();
}



void loop()
{
  static int i=0;
  char char_command[100];
  
  //char command[]="mdc_layer1_preset";
  char command[]="mdc_layer1_preset_next";
  
    unsigned long currentMillis = millis();
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
    {
        wificonnect();
    }
    MQTT_connect();
    if (ButtonPressed > 0)
    {
        Serial.println("Button pressed!");      
        String s="";
        //s = s+command+presetlist[i]; //use this for preset numbers from presetlist, change the command[] above too!
        s = s+command; //use this for "normal" osc commands
        s.toCharArray(char_command,100);
        
        cmd.publish(char_command);
        Serial.println("Publish "+String(char_command));
        ButtonPressed = 0; 
        i++;
        if(i>sizeof(presetlist)/sizeof(presetlist[0])-1) i=0;
    }
    delay(25);
}
