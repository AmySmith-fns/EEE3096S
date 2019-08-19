import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

GPIO.setup(12, GPIO.OUT)
def main():
	while 1==1:
		GPIO.output(12, 1)
		time.sleep(2)

if __name__=="__main__":
	try:
		while True:
			main()
	except KeyboardInterrupt:
		GPIO.cleanup()


