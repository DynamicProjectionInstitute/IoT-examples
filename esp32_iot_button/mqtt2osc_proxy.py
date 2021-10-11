#!/usr/bin/env python3

# Copyright (c) 2021 by Dynamic Projeciton Institute GmbH, Vienna, Austria
# Author: Martin Willner <willner@dynamicprojection.com>

# MDC-X setup
# sudo apt-get install python3-pip
# sudo apt-get install python3-setuptools
# pip3 install paho-mqtt

# run on the MDC-X with: python3 mqtt2osc_proxy.py 

import socket,sys
import paho.mqtt.subscribe as subscribe
from OSC import OSCClient, OSCMessage
import ast

#CONFIG STARTS HERE
MQTT_SERVER="127.0.0.1" 
MYUUID="1234"

# USAGE
# All OSC commands are available over MQTT
# To trigger an OSC command over MQTT (this is a 1:1 proxy ) use i.e.
#
# for BASIC OSC COMMANDS i.e.:
# mosquitto_pub -h MQTT_SERVER -t "mdcx/MYUUID/osc" -m "mdc_layer1_preset_next"
#
# for ADVANCED OSC COMMANDS i.e:
# mosquitto_pub -h MQTT_SERVER -t "mdcx/MYUUID/osc" -m "mdc_dmx (1,1,255,2,255,3,255,4,255)"


#CONFIG ENDS HERE



MDCX_HOST="127.0.0.1"
MDCX_OSC_PORT=7475

state_preset=0
state_dmx=0

mdcx_osc = OSCClient()
mdcx_osc.connect((MDCX_HOST, MDCX_OSC_PORT))

def oscsender(obj,topic,msg):
        try:
                if msg is None:
                        obj.send(OSCMessage(topic))
                        print("OSC LOG: TOPIC("+str(topic)+")")
                else:
                        obj.send(OSCMessage(topic,msg))
                        print("OSC LOG: TOPIC("+str(topic)+") MSG("+str(msg)+")")
        except:
                print("OSC ERROR - failed to send!")



def on_message(client, userdata, message):
    global state_preset
    uid = message.topic.split("/")[1]
    topic = message.topic.split("/")[2]

    if uid == MYUUID or uid == "global":
        print("MQTT RECEIVED => TOPIC: \"%s\"  PAYLOAD: \"%s\"" % (message.topic, message.payload))
    else:
        return


    if topic == "osc":
        stri="/"+message.payload.decode("utf-8") 
        m=stri.split(" ")
        if len(m) > 1:
            try:
                meval = ast.literal_eval(m[1])
                oscsender(mdcx_osc,m[0],meval)
            except:
                oscsender(mdcx_osc,m[0],1.0)
        else:
            oscsender(mdcx_osc,m[0],1.0)


print("Connect to MQTT server:",MQTT_SERVER)
subscribe.callback(on_message, "mdcx/#", hostname=MQTT_SERVER)
