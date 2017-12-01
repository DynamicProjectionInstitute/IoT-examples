# Simple mqtt to MDC-X OSC converter

# Copyright (c) 2017 by Dynamic Projeciton Institute GmbH, Vienna, Austria
# Author: Martin Willner <willner@dynamicprojection.com>



import socket
import paho.mqtt.subscribe as subscribe

from OSC import OSCClient, OSCMessage

MDCX_HOST="127.0.0.1"
MDCX_OSC_PORT=7475
MQTT_SERVER="2.0.0.20" #use a JCD as MQTT broker


state_preset=0
state_dmx=0

mdcx_osc = OSCClient()
mdcx_osc.connect((MDCX_HOST, MDCX_OSC_PORT))

def oscsender(obj,topic,msg):
        try:
                if msg is None:
                        obj.send(OSCMessage(topic))
                        print "OSC LOG: TOPIC("+str(topic)+")"
                else:
                        obj.send(OSCMessage(topic,msg))
                        print "OSC LOG: TOPIC("+str(topic)+") MSG("+str(msg)+")"
        except:
                print "OSC ERROR - failed to send!"


def on_message_print1(client, userdata, message):
    global state_preset
    print "CMD to send..."
    print("%s - %s" % (message.topic, message.payload))

    if "dips/2/sensor" == message.topic :
        if state_preset != int(message.payload):
                state_preset = int(message.payload)
                pr=int(message.payload)+1
                stri="/mdc_layer1_preset"+str(pr)
                oscsender(mdcx_osc,stri,1.0)

    if "dips/1/sensor" == message.topic :
        pr=int(message.payload)/1024.0/2.0
        stri="/position1/state"
        oscsender(mdcx_osc,stri,[pr,0.5])

print "Connect to MQTT server:",MQTT_SERVER
subscribe.callback(on_message_print1, "dips/#", hostname=MQTT_SERVER)

