from arduino import Arduino
import time

a = Arduino("COM11") # get the Arduino device on COM port 11
a.digital[3].setMode(Arduino.OUTPUT) # equivalent to the setMode() Arduino function

# blink pin 3 on/off
while(True):
	a.digital[3].write(1) # equivalent to the digitalWrite() Arduino function
	time.Sleep(1)
	a.digital[3].write(0)
	time.Sleep(1)