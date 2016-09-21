# CapeTest.py
# 09/21/2016

import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_10", GPIO.OUT)
GPIO.setup("P8_11", GPIO.OUT)

GPIO.setup("P8_12", GPIO.IN)
GPIO.setup("P8_15", GPIO.IN)

oldSwitchState12 = 1
oldSwitchState15 = 1

print('LED test...')
for x in range(15):
	GPIO.output("P8_10", GPIO.HIGH)
	GPIO.output("P8_11", GPIO.LOW)
	time.sleep(x/10.0)

	GPIO.output("P8_10", GPIO.LOW)
	GPIO.output("P8_11", GPIO.HIGH)
	time.sleep(x/10.0)

GPIO.output("P8_11", GPIO.LOW)

print('Switches active...')
while True:
	newSwitchState12 = GPIO.input("P8_12")
	newSwitchState15 = GPIO.input("P8_15")
	
	if newSwitchState12 == 0 and oldSwitchState12 == 1:
		print('Switch12')
		GPIO.output("P8_10", GPIO.HIGH)
	
	if newSwitchState15 == 0 and oldSwitchState15 == 1:
		print('Switch15')
		GPIO.output("P8_11", GPIO.HIGH)
	
	time.sleep(1.0)
	
	GPIO.output("P8_10", GPIO.LOW)
	GPIO.output("P8_11", GPIO.LOW)
	
	oldSwitchState12 = newSwitchState12
	oldSwitchState15 = newSwitchState15
