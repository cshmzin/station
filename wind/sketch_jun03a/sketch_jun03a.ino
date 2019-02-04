#define FX  A0
#define FS  4
double wspeed;
double wdiection;
volatile long count=0;


void blinkA()
{
  count++;
}
double getFX()
{
  int diection = analogRead(FX);
  wdiection =diection * (5.0/1024.0) *72;
  return wdiection;
}

double getFS()
{
  if(count==0){wspeed=0;}
  else{wspeed=0.1+0.0875*(count/2);}
  count=0;
  return wspeed;
}


double pm25 = 0;
double pm10 = 0;

void getPMInfo()
{
  uint8_t check = 0;
  uint8_t data[6] = {0};
  uint8_t sum = 0;
  while(Serial.available() > 0)
  {
    check = Serial.read();
    delay(2);
    if(check == 0xAA)
    {
      check = Serial.read();
      if(check == 0xC0)
      {
        for(int i = 0; i < 6; i++)
        {
          data[i] = Serial.read();
          delay(2);
          sum += data[i];
        }
        check = Serial.read();
        delay(1);
        if( check == sum && Serial.read() == 0xAB)
        {
          pm25 = (double)((int)data[0] | (int)data[1]<<8);
          pm10 = (double)((int)data[2] | (int)data[3]<<8);
          pm25 /= 10;
          pm10 /= 10;
        }
      }
    }
  }
}


void input()
{
  char* dirstring[16] = {"North","North1","Northeasterly","Northeasterly1","East","East1","Southeast",
                       "Southeast1","South","South1","Southwest","Southwest1","West","West1","Northwest","Northwest1"};

   Serial.print("windspeed: %.2lf m/s,",getFS());
   Serial.print("winddirection : ");
   if (getFX() >= 348.75|| getFX() <11.25 )
   { Serial.print( dirstring[0] );}
   else if(getFX() >=11.25 && getFX() < 33.75)
   { Serial.print( dirstring[1] );}
   else if(getFX() >=33.75 && getFX() < 56.25)
   { Serial.print( dirstring[2] );}
   else if(getFX() >=56.25 && getFX() < 78.75)
   { Serial.print( dirstring[3] );}
   else if(getFX() >=78.75 && getFX() < 101.25)
   { Serial.print( dirstring[4] );}
   else if(getFX() >=101.25 && getFX() <123.75)
   { Serial.print( dirstring[5] );}
   else if(getFX() >=123.75 && getFX() <146.25)
   { Serial.print( dirstring[6] );}
   else if(getFX() >=146.25 && getFX() <168.75)
   { Serial.print( dirstring[7] );}
   else if(getFX() >=168.75 && getFX() <191.25)
   { Serial.print( dirstring[8] );}
   else if(getFX() >=191.25 && getFX() <213.75)
   { Serial.print( dirstring[9] );}
   else if(getFX() >=213.75 && getFX() <236.25)
   { Serial.print( dirstring[10] );}
   else if(getFX() >=236.25 && getFX() <258.75)
   { Serial.print( dirstring[11] );}
   else if(getFX() >=258.75 && getFX() < 281.25)
   { Serial.print( dirstring[12] );}
   else if(getFX() >=281.25 && getFX() < 303.75)
   { Serial.print( dirstring[13] );}
   else if(getFX() >=303.75 && getFX() < 326.25)
   { Serial.print( dirstring[14] );}
   else if(getFX() >=326.25 && getFX() < 348.75)
   { Serial.print( dirstring[15] );} 
   Serial.print(",");
   Serial.print("PM2.5:");
   Serial.print(pm25);
   Serial.print("PM1.0:");
   Serial.print(pm10);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(FS,INPUT);
  pinMode(FX,INPUT);
  attachInterrupt(FS,blinkA,FALLING);
}
void loop() 
{
  input();
  delay(2000);
}
