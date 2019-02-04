#!/usr/bin/python
#coding=utf-8
import time
import datetime
import os
from BMP180 import BMP180
from BH1750 import get_Illuminance
from DHT1 import get_humidity
from mq import get_mq135
# Initialise the BMP085 and use STANDARD mode (default value)
# bmp = BMP085(0x77, debug=True)
bmp = BMP180()
f = open('data.txt', "a")
while True:
    
    temperature = bmp.read_temperature()
    pressure = bmp.read_pressure()
    humidity = get_humidity()
    illuminance = get_Illuminance()
    iftoxic = get_mq135()
    nowTime=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')#现在
    print nowTime
    print "Temperature: %.2f ℃" % temperature
    print "humidity:    %.2f %%" % humidity
    print "Pressure:    %.2f hPa" % (pressure / 100.0)
    print "illuminance: %.2f lx\n" % illuminance
    print >> f,"Time: %s,Temperature: %.2f ℃ ,Humidity: %.2f %% ,Pressure: %.2f hPa ,Illuminance: %.2f lx,iftoxic: %s" % (nowTime,temperature,humidity,pressure,illuminance,iftoxic)
    time.sleep(599)

f.close()
