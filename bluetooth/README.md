# MH-06 Bluetooth Adapter to MDC-X1/2 control example

## Overview

Overview of communication of all devices
```
 [Arduino]<-RX/TX->[HC-06]  <~~~Bluetooth~~~> [MDC-X]--OSC-->(MDC)
```


## Wiring

Tested with Arduino Nano (ATMega328p). Adjust the SoftSerial settings for your MCU if needed!

```
HC-06 RDX  -> Arduino D11 (=TX)
HC-06 TDX  -> Arduino D10 (=RX)
HC-06 GND  -> Arduino GND
HC-06 VCC  -> Arduino +5V
```

## Arduino setup

Compile and flash the Arduino sketch Arduino-hc06.ino to the Arduino device. Make sure all wiring is correct.

## Bluetooth setup on the MDC-X side

Make sure the bluetooth stack is enabled.
- Pair the HC-06 device by eighter using the bluetooth manager or do it in the command line (example below). In both cases the PIN for pairing is "1234". After you paired the device make sure you bind it to the serial rfcomm0!

- Linux bluetooth Setup with bluetoothctl. Note that the MAC "98:D3:31:FB:4E:98" is just an example! You will have a different one!
```
  ~$ bluetoothctl
  [bluetooth]# agent on
  [bluetooth]# power on
  [bluetooth]# scan on
  [bluetooth]# devices
  Device 98:D3:31:FB:4E:98 HC-06
  [bluetooth]# pair 98:D3:31:FB:4E:98
  Attempting to pair with 98:D3:31:FB:4E:98
  [CHG] Device 98:D3:31:FB:4E:98 Connected: yes
  Request PIN code
  [agent] Enter PIN code: 1234
  [CHG] Device 98:D3:31:FB:4E:98 Paired: yes
  Pairing successful
  [CHG] Device 98:D3:31:FB:4E:98 Connected: no
  [bluetooth]# quit
  ~$ sudo rfcomm bind 0 98:D3:31:FB:4E:98
  ~$ sudo chmod a+rwx /dev/rfcomm0
 ```
## Running the rfcomm to osc converter

- Start the rfcomm2osc.py program at the shell
```
~$ python rfcomm2osc.py
```

You should now see the messages from the HC-06 arriving.

## NO WARRANTY

BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.



*Copyright (c) by Dynamic Projection Institute GmbH*
