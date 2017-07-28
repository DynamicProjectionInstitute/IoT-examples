/*
Example for Bluetooth connection of ARDUINO and MDC-X
Requirements:
HC-06 Bluetooth Module
Arduino (tested with Arduino Nano)
MDC-X Linux System

PLEASE PAY ATTENTION TO THE PINS

HC-06 RDX  -> Arduino D11 (=TX)
HC-06 TDX  -> Arduino D10 (=RX)
HC-06 GND -> Arduino GND
HC-06 VCC -> Arduino +5V


Copyright (c) by Dynamic Projection Institute GmbH
Author: Martin Willner <willner@dynamicprojection.com>
*/

#include "SoftwareSerial.h"

SoftwareSerial hc06serial(10, 11);  // (RX,TX)

#define DEBUG 1
#define BUFFER_SIZE 64

char inData[BUFFER_SIZE];
char inChar = -1;
int count = 0;
int i = 0;

void setup()
{
  Serial.begin(9600);  // DEBUG CONSOLE !
  hc06serial.begin(9600); // HC-06 Serial over BT
  hc06serial.println(0x00); // write "something"
  #ifdef DEBUG
  Serial.println("*** SERIAL INIT COMPLETED ***");
  #endif
  hc06serial.println("*** Ping MSG to MDC-X ***");

}
void loop()
{

  byte bc = 0;

  // Sending some preset change commands to the MDC-X
  for(int j=1; j<=3;j++) {
    hc06serial.print("/mdc_layer1_preset");
    hc06serial.println(j);
    delay(2000);
  }

  // Sending some media change commands to the MDC-X
  for(int j=1; j<=3;j++) {
    hc06serial.print("/mdc_layer1_media");
    hc06serial.println(j);
    delay(2000);
  }

  bc = hc06serial.available();
  if (bc > 0) {
    for(i=0;i<bc;i++) { inData[i] = hc06serial.read(); }
    inData[i] = '\0';
    #ifdef DEBUG
    Serial.print("Incoming MSG(");
    Serial.print(bc);
    Serial.print("):");
    Serial.println(inData);
    #endif
  }
  delay(100);
}
