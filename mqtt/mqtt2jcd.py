# Simple MQTT to JCD message converter
# 
# To test it: mosquitto_pub -h MQTT_SERVER -t "jcd/m2j/global/cmd" -m "ON"
#
# Copyright (c) 2017 by Dynamic Projeciton Institute GmbH, Vienna, Austria
# Author: Martin Willner <willner@dynamicprojection.com>

import socket
import platform
import paho.mqtt.client as mqtt

#jcd_host="127.0.0.1"  
jcd_host="2.0.0.20"
jcd_port=7781
MQTT_SERVER="2.0.0.20" 

hostname=platform.node()

jcd_id=hostname  # change this if you need a static id

jcd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def on_message_print(client, userdata, message):
    print "JCD CMD to send..."
    print("%s %s" % (message.topic, message.payload))
    jcd.sendto(message.payload, (jcd_host, jcd_port))

client = mqtt.Client()
client.on_message=on_message_print
client.connect(MQTT_SERVER)
client.subscribe("jcd/m2j/"+str(hostname)+"/cmd") #listen on own ID
client.subscribe("jcd/m2j/global/cmd")  #listen on global instructions
 
client.loop_forever()

