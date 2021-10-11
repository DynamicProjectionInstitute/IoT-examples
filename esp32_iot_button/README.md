# ESP32 IOT Button Trigger

## Scenario

The ESP32 device with a button triggers an OSC command which is published as a MQTT message to a MQTT-Broker (JCD) where a MDC-X device is subscribed. 

ESP32 (publisher) =(MQTT)=> JCD (MQTT BROKER) <=(MQTT)= MDC-X ( mqtt2osc_proxy.py , subscriber) =(OSC)=> MDC program

To use it without the JCD Broker (MDC-X as broker) you have to change the settings in the .ino and the .py file. The scenatio then is:

ESP32 (publisher) =(MQTT)=> MDC-X (MQTT BROKER) <= (MQTT)= MDC-X ( mqtt2osc_proxy.py , subscriber) =(OSC)=> MDC program

## Install 

### ESP32

Flash the .ino file to the esp32 device. Please check if the button_pin is correct and adjust the WiFi settings of the JCD device. If you do not use a JCD as MQTT-Broker use the WiFi settings of your Network. 
When you press the button on the ESP32 a message will be published. See the Serial-monitor log for more information.

### MDX-X

Install `sudo apt-get install python3-pip python-setuptools` and `pip3 install paho-mqtt` on the MDC-X device. The MQTT Server should be the JCD device, so use the default IP 2.0.0.20 for a vanilla setup. if the MDC-X is the broker install `sudo apt-get install mosquitto mosquitto-clients mosquitto-dev`. Start with `python3 mqtt2osc_proxy.py` 




