import RPi.GPIO as GPIO

import time

def get_mq135():
	CHANNEL=11
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(CHANNEL, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    defaction(pin):
    mq135='有毒'
    GPIO.add_event_detect(CHANNEL, GPIO.RISING)
    GPIO.add_event_callback(CHANNEL, action)
    try:
      whileTrue:
      mq135='安全'
    except KeyboardInterrupt:
      GPIO.cleanup()
    return mq135

	
	
