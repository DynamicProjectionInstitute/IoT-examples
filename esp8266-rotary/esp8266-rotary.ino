/*******************************************************************************/
/*macro definitions of PIR motion sensor pin and LED pin*/

#include <Encoder.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "JCD-e211c65b"
#define WLAN_PASS       "b19dcdc9"
#define AIO_SERVER      "192.168.1.1"
#define AIO_SERVERPORT  1883        
#define AIO_USERNAME    ""
#define AIO_KEY         ""

#define PIR_MOTION_SENSOR D4
#define LED    16

static bool ledStatus = false;
static bool state=false;
static char st[8];

bool shutter_state = false;

WiFiClient wificlient;
Adafruit_MQTT_Client mqtt(&wificlient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish cmd = Adafruit_MQTT_Publish(&mqtt, "jcd/JCD-e211c65b/jcd/cmd");

void MQTT_connect();

long oldPosition  = -999;
Encoder myEnc(D1, D2);

boolean isButtonPressed = false;
long lastUpdateMillis = 0;

void lighttoggle() {
  ledStatus = !ledStatus;
  digitalWrite(LED_BUILTIN,ledStatus);
  Serial.println(".");
  delay(500);
}

void scan_wifi() {

   int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}


void wificonnect() {
  Serial.println("wificonnect");
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    lighttoggle();
  }
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN,true);
}


void MQTT_connect() {
 int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
               while (1);
       }
  }
  Serial.println("MQTT Connected!");

}


void handleKey() {
  isButtonPressed = true;
  
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Test...");

  pinMode(D3, INPUT_PULLUP);
  attachInterrupt(D3, handleKey, RISING);
  pinsInit();
  printf(st,"%x",ESP.getChipId());
  scan_wifi();
  wificonnect();
 
}

void loop()
{
  MQTT_connect();
  long newPosition = myEnc.read();
  int motion = digitalRead(PIR_MOTION_SENSOR);
  if ( motion == HIGH)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }

  if (newPosition != oldPosition) {
    if(newPosition - oldPosition > 0) {
      cmd.publish("F+");
    } else {
      cmd.publish("F-");
    }
    oldPosition = newPosition;
    Serial.print("Encoder: ");
    Serial.println(newPosition);
   
    cmd.publish((int)newPosition);
  }

  if (isButtonPressed && millis() - lastUpdateMillis > 50) {
    isButtonPressed = false;
    Serial.println("Button Pressed!");
    if(shutter_state) {
    Serial.println("ON!");
       cmd.publish("AVMUTE");

       cmd.publish("SHON");
    } else {
    Serial.println("OFF!");
       cmd.publish("AVUNMUTE");
       cmd.publish("SHOFF");

    }
    shutter_state = !shutter_state;
    lastUpdateMillis = millis();
  }


  delay(10);

}
void pinsInit()
{

  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
  }

}

