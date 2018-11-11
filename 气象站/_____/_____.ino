/*  
 *  * CONNECTIONS:
 *      DS1302:
 *          DS1302 RST --> GPIO0
 *          DS1302 DAT --> GPIO2
 *          DS1302 CLK --> GPIO15
 *          DS1302 VCC --> 3.3V
 *          DS1302 GND --> GND
 *      BH1750:
 *          VCC --> 5V
 *          GND --> GND
 *          SCL --> GPIO 05
 *          SDA --> GPIO 04
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DS1302.h>
#include <Esp.h>
#include <IOTPWebSocketsClient.h>
#include <string.h>
#include <stdlib.h>
#include<Wire.h>
#include<math.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
#define SSID    "Honor"
#define PWD     "826926csh"
#define HEART_BEAT "HEART_BEAT"
#define RST         0
#define DAT         2
#define CLK         15
            
DS1302 rtc(RST,DAT,CLK);
IOTPWebSocketsClient webSocket("211b4f57953641ba94e38f9b81e8268a",88888);

int BH1750address = 0x23;
byte buff[2];
double getBH = 0;
double getTem = 0;
double getHum = 0;
double Temperature = 0;
double Pressure=0;
double val=0;

void getTemperature()
{
   Temperature=bmp.readTemperature();
}

void getPressure()
{
  Pressure = bmp.readPressure();
}

void getPM()
{
  val = analogRead(0);
}

void getBH1750()
{
   int i=0;
  //开始I2C读写操作
  Wire.beginTransmission(BH1750address);
  Wire.write(0x10);//发送命令
  Wire.endTransmission();  
  
  delay(200);
  //读取数据
  Wire.beginTransmission(BH1750address);
  Wire.requestFrom(BH1750address, 2);
  while(Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  if(2==i)
  {
   getBH=((buff[0]<<8)|buff[1])/1.2;
  }
}

char * getTimeInfo()
{
    Time t;
    t=rtc.getTime();
    char * output = "2017-11-11 11:11:11";
    output[0]=char((t.year / 1000)+48);
    output[1]=char(((t.year % 1000) / 100)+48);
    output[2]=char(((t.year % 100) / 10)+48);
    output[3]=char((t.year % 10)+48);
    output[4]='-';
    if(t.mon<10)
        output[5] = 48;
    else
        output[5] = char((t.mon / 10) + 48);
    output[6]=char((t.mon % 10)+48);
    output[7]='-';
    if (t.date<10)
        output[8]=48;
    else
        output[8]=char((t.date / 10)+48);
    output[9]=char((t.date % 10)+48);
    output[10]=0;
    strcat(output," ");
    strcat(output,rtc.getTimeStr());
    return output;
}


void webSocketEvent(WStype_t type,uint8_t * payload,size_t length)
{
  switch(type)
  {
    case WStype_DISCONNECTED:
        {
            Serial.printf("[FAIL]%s: Disconnected to server!\n", getTimeInfo());
            if(WiFi.status() != WL_CONNECTED)
            {
                Serial.printf("[failed]%s:Disconnect to WiFi\n",getTimeInfo());
            }
            Serial.printf("Reconnecting");
            while(WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
            }
        }
        break;
    case WStype_CONNECTED: 
    {
        Serial.printf("[OK]%s: Connected to url: %s\n", getTimeInfo(), payload);
        
        // send message to server when Connected
        webSocket.sendTXT("IOTPWebSocketClient test!", getTimeInfo());
    }
        break;
    case WStype_TEXT:
    {
        if(strcmp((char *)payload,HEART_BEAT) == 0)
        {

            char gpinfo[] = "{\\\"GZ\\\": 00.00 lx,\\\"Temperature\\\":00.00 *C,\\\"Pressure\\\":00.00 Pa,\\\"PM\\\":00.00}";

            getBH1750();
            getTemperature();
            getPressure();
            getPM();
            
            sprintf(gpinfo,"{\\\"GZ\\\": %.2lf lx,\\\"Temperature\\\":%.2lf *C,\\\"Pressure\\\":%.2lf Pa,\\\"PM\\\":%.2lf}",getBH,Temperature,Pressure,val);
            Serial.printf("%.2lf\n",getBH);
            Serial.printf("%.2lf\n",Temperature);
            Serial.printf("%.2lf\n",Pressure);
            Serial.printf("%.2lf\n",val);
            // send message to server
            webSocket.sendTXT(gpinfo, getTimeInfo());
        }
        else
        {
            Serial.printf("[Message]%s: get message: %s\n", getTimeInfo(), payload);
        }
    }
    break;
  default:
    {
        Serial.printf("Default!\n");
    }
    break;
  }

}

void setup()
{
    Wire.begin();
    Serial.begin(9600);
      if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

    rtc.halt(false);
    rtc.writeProtect(false);

    Serial.println(getTimeInfo());
    getBH1750();
    Serial.printf("%.2lf lx\n",getBH);
    Serial.print("Attempting to connect to ");
    Serial.println(SSID);
    WiFi.begin(SSID, PWD);
    Serial.print("Connecting");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to wifi");
    webSocket.begin();

    webSocket.onEvent(webSocketEvent);

    webSocket.setReconnectInterval(2);
}

void loop()
{
    webSocket.loop();
}
