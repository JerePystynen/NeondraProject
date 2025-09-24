#!/usr/bin/env python
import serial
import time

ser = serial.Serial(
	port='/dev/ttyS0',
	baudrate=9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1
)

try:
	while True:
		# READ received data.
		if ser.in_waiting > 0:
			line = ser.readline().decode('utf-8').rstrip()
			if "esp32:" in line:
				print(line)
		# WRITE data.
		data = "pi:emoticon=1&set_color=red&sfx=2"
		ser.write(data.encode())
		# delay.
		time.sleep(1)
except KeyboardInterrupt:
	print("ESP32 communication program terminated!")
finally:
	ser.close()
