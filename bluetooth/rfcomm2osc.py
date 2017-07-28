# Sample for remote control the MDC-X system by OSC over Bluetooth
#
# Copyright (c) 2017 by Dynamic Projeciton Institute GmbH, Vienna, Austria
# Author: Martin Willner <willner@dynamicprojection.com>
#
#
#
# Linux bluetooth Setup with bluetoothctl:
# ~$ bluetoothctl
# [bluetooth]# agent on
# [bluetooth]# power on
# [bluetooth]# scan on
# [bluetooth]# devices
# Device 98:D3:31:FB:4E:98 HC-06
# [bluetooth]# pair 98:D3:31:FB:4E:98
# Attempting to pair with 98:D3:31:FB:4E:98
# [CHG] Device 98:D3:31:FB:4E:98 Connected: yes
# Request PIN code
# [agent] Enter PIN code: 1234
# [CHG] Device 98:D3:31:FB:4E:98 Paired: yes
# Pairing successful
# [CHG] Device 98:D3:31:FB:4E:98 Connected: no
# [bluetooth]# quit
# ~$ sudo rfcomm bind 0 98:D3:31:FB:4E:98
# ~$ sudo chmod a+rwx /dev/rfcomm0
#

import serial, time
from OSC import OSCClient, OSCMessage

MDCX_HOST="192.168.0.200"
MDCX_OSC_PORT=7475

rfcomm_device="/dev/rfcomm0"

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

print("Start reading...")
rfcomm=serial.Serial(rfcomm_device, 9600)
rfcomm.flushInput()

while True:
	rfdata=rfcomm.readline()
	s=rfdata.decode()
	# send received string to the OSC receiver
	oscsender(mdcx_osc,s[:len(s)-2],"")
rfcomm.close()
