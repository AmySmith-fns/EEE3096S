#!/usr/bin/python3

#Mini Project A
# EEE3096S
# RJHINE001 and SMTAMY005

import spidev  #for SPI
import time
import smbus #for I2C
import RPi.GPIO as GPIO

#---------global variables
#---I2C
i2c_ch=1
i2c_ad=0x6f
reg_ad=0x01
bus= smbus.SMBus(i2c_ch)
ADC= spidev.SpiDev()
ADC_ad=0x4

DAC= spidev.SpiDev()
DAC_ad=0x02
#------------
stsec=0
stmin=0
sthours=0

def main():
	print("Write your shit here")
	setupstuff()
	print(stsec)
	print(stmin)
	print(sthours)
	time.delay(1)

def setupspi():
#setup the ADC
	global ADC
	ADC= spidev.SpiDev()
#	ADC.open(0,0)
	#adc.readbytes(n bytes)
	#adc.writebytes()
#setup the DAC
#	global DAC= spidev.SpiDev()
#	DAC.open(0,1)
#How to write to DAC using SPI:
#to_send = [0x01, 0x02, 0x03]
#spi.xfer(to_send)

def setupi2c():
	global RTC
	RTC= smbus.SMBus(i2c_ch)
#val=RTC.read/write_i2c_block_data(i2caddress, registeraddress, num bits/ value to write)
def setupstuff():
#read start time
	print('line1')   
	stsec=bus.read_byte(i2c_ad)
	print('read secs')
	print(stsec)
	stmin=bus.read_i2c_block_data(i2c_ad, 0x01,8)

	sthours=bus.read_i2c_block_data(i2c_ad, 0x02,8)



if __name__ == "__main__":
	try:
		while True:
			main()
	except KeyboardInterrupt:
		print("Laterz")
		GPIO.cleanup()
	except Exception as  e:
		GPIO.cleanup()
		print("Y'all fucked up")
		print(e.message)


