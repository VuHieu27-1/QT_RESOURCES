#include <Adafruit_AM2315.h>
#include <SoftwareSerial.h>
#include <BH1750.h>
#include "MQ135.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
SoftwareSerial Atmega328p(2, 4);
LiquidCrystal_I2C lcd(0x27, 16, 4);
BH1750 lightMeter;
Adafruit_AM2315 am2315;
#define air  A2
#define amdat A3
#define button1 9
#define button2 13
#define button3 12
#define button4 11
#define button5 10
#define bom     7
#define den     8
#define quat    6
#define maichethuan A0
#define maichenghich  A1
MQ135 mq135_sensor = MQ135(air);
int resetPin = 5;
int automode1, automode2;
int interruptPin = 3;
int k, q, nhieu;
String data, tach, amdat1, anhsang1, air1, am1, am2 ;
String value[6];
const char airr[] = {13, 18, 0, 13, 18, 0, 13, 18 };
const char suong[] = {4, 4, 10, 17, 17, 17, 10, 4 };
const char dad[] = {4, 14, 31, 31, 4, 4, 14, 21 };
const char asang[] = {21, 0, 14, 10, 14, 0, 21, 0 };
const char bomm[] = {14, 31                      , 0, 21, 0, 21, 0, 21 };
const char quatt[] = {19, 26, 4, 11, 25, 4, 4, 4 };
const char denn[] = {14, 17, 17, 17, 21, 14, 14, 14 };
const char maichee[] = {31, 30, 28, 25, 19, 7, 15, 31 };
void setup()
{
  Serial.begin(9600);
  Atmega328p.begin(9600);
  Wire.begin();
  am2315.begin();
  lightMeter.begin();
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, airr);
  lcd.createChar(3, suong);
  lcd.createChar(4, dad);
  lcd.createChar(5, asang);
  lcd.createChar(7, bomm);
  lcd.createChar(0, quatt);
  lcd.createChar(6, denn);
  lcd.createChar(2, maichee);
  pinMode(button1, INPUT);
  pinMode(bom, OUTPUT);
  pinMode(button2, INPUT);
  pinMode(den, OUTPUT);
  pinMode(button3, INPUT);
  pinMode(quat, OUTPUT);
  pinMode(button4, INPUT);
  pinMode(maichethuan, OUTPUT);
  pinMode(button5, INPUT);
  pinMode(maichenghich, INPUT);
  pinMode(amdat, INPUT);
  pinMode(air, INPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ngat1, FALLING);
}
/*void haman()
{ 
    Serial.println(q);
    nhieu = 0;
   if(tach=="1"&&(q==0||q==1))
   {
    tach="1";
    q++;
    }
    else
    {
      tach="0";
      q=0;
      }
}*/
void reset1()
{
  if (k == 0)
  {
    digitalWrite(resetPin, HIGH);
    delay(500);
    digitalWrite(resetPin, LOW);
    k++;
    lcd.setCursor(2, 1);
    lcd.print("He Thong IOT");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Vui Long Cho ...");
    delay(4000);
    lcd.clear();
  }
}
void ngat1()
{
  delay(30);
  if ( !digitalRead(button1) && !nhieu)
  {
    if ( value[0] == "1")
    {
      value[0] = "0";
    }
    else
    {
      value[0] = "1";
    }
    nhieu++;
  }
  if ( !digitalRead(button2) && !nhieu)
  {
    if ( value[1] == "1")
    {
      value[1] = "0";
    }
    else
    {
      value[1] = "1";
    }
    nhieu++;
  }
  if ( !digitalRead(button3) && !nhieu)
  {
    if ( value[2] == "1")
    {
      value[2] = "0";
    }
    else
    {
      value[2] = "1";
    }
    nhieu++;
  }
  if ( !digitalRead(button4) && !nhieu)
  {
    if (value[3] == "1" && value[5] == "0")
    {
      value[3] = "0";
      value[5] = "1";
    }
    else if (value[3] == "0" && value[5] == "1")
    {
      value[3] = "0";
      value[5] = "0";
    }
    else
    {
      value[3] = "1";
      value[5] = "0";
    }
    nhieu++;
  }
  if ( !digitalRead(button5) && !nhieu)
  {
    if ( value[4] == "1")
    {
      value[4] = "0";
    }
    else
    {
      value[4] = "1";
    }
    nhieu++;
  }
  q=0;
  tach = "1";
}
void nut1()
{
  lcd.setCursor(-4, 2);
  lcd.print(char(7));
  if (value[0] == "1")
  {
    lcd.print("ON ");
    digitalWrite(bom, HIGH);
  }
  else
  {
    lcd.print("OFF");
    digitalWrite(bom, LOW);
  }
}
void nut2()
{
  lcd.setCursor(8, 2);
  lcd.print(char(6));
  if (value[1] == "1")
  {
    lcd.print("ON ");
    digitalWrite(den, HIGH);
  }
  else
  {
    lcd.print("OFF");
    digitalWrite(den, LOW);
  }
}
void nut3()
{
  lcd.setCursor(2, 2);
  lcd.print(char(0));
  if (value[2] == "1")
  {
    lcd.print("ON ");
    digitalWrite(quat, HIGH);
  }
  else
  {
    lcd.print("OFF");
    digitalWrite(quat, LOW);
  }
}
void nut4()
{
  lcd.setCursor(-4, 3);
  lcd.print(char(2));
  if (value[4] == "1")
  {
    lcd.print("ON ");
    digitalWrite(maichethuan, HIGH);
    digitalWrite(maichenghich, LOW);
  }
  else
  {
    lcd.print("OFF");
    digitalWrite(maichethuan, LOW);
    digitalWrite(maichenghich, HIGH);
  }
}
void nut5()
{
  lcd.setCursor(2, 3);
  lcd.print("AUTO:");
  if (value[3] == "1")
  {
    lcd.print("ON1");
    automode1 = 1;
    automode2 = 0;
  }
  else if (value[5] == "1")
  {
    lcd.print("ON2");
    automode1 = 0;
    automode2 = 1;
  }
  else
  {
    lcd.print("OFF");
    automode1 = 0;
    automode2 = 0;
  }
}
void chedotudong()
{
  if (automode1 == 1)
  {
    lcd.setCursor(-4, 2);
    lcd.print(char(7));
    lcd.print("NO!");
    lcd.setCursor(8, 2);
    lcd.print(char(6));
    lcd.print("NO!");
    lcd.setCursor(2, 2);
    lcd.print(char(0));
    lcd.print("NO!");
    lcd.setCursor(-4, 3);
    lcd.print(char(2));
    lcd.print("NO!");
    digitalWrite(den, LOW);
    if (amdat1.toInt() < 20)
    {
      digitalWrite(bom, HIGH);
    }
    else
    {
      digitalWrite(bom, LOW);
    }
    if (am1.toInt() < 90)
    {
      digitalWrite(bom, HIGH);
    }
    else
    {
      digitalWrite(bom, LOW);
    }
    if (am2.toInt() > 30)
    {
      digitalWrite(quat, HIGH);
    }
    else
    {
      digitalWrite(quat, LOW);
    }
    if (air1.toInt() > 40)
    {
      digitalWrite(quat, HIGH);
      digitalWrite(maichethuan, HIGH);
    }
    else
    {
      digitalWrite(quat, LOW);
      digitalWrite(maichenghich, HIGH);
    }
  }
  else if (automode2 == 1)
  {
    lcd.setCursor(-4, 2);
    lcd.print(char(7));
    lcd.print("NO!");
    lcd.setCursor(8, 2);
    lcd.print(char(6));
    lcd.print("NO!");
    lcd.setCursor(2, 2);
    lcd.print(char(0));
    lcd.print("NO!");
    lcd.setCursor(-4, 3);
    lcd.print(char(2));
    lcd.print("NO!");
    digitalWrite(quat, LOW);
    digitalWrite(maichenghich, HIGH);
    if (amdat1.toInt() < 20)
    {
      digitalWrite(bom, HIGH);
    }
    else
    {
      digitalWrite(bom, LOW);
    }
    if (am1.toInt() < 90)
    {
      digitalWrite(bom, HIGH);
    }
    else
    {
      digitalWrite(bom, LOW);
    }
    if (anhsang1.toInt() < 150)
    {
      digitalWrite(den, HIGH);
    }
    else
    {
      digitalWrite(den, LOW);
    }
  }
  else
  {
    nut1();
    nut2();
    nut3();
    nut4();
  }
}
void doamdat()
{
  int amdatvalue;
  int value = analogRead(amdat);
  amdatvalue = map(value, 585, 360, 0, 100);
  Serial.print("Do am dat: ");
  Serial.println(amdatvalue);
  amdat1 = amdatvalue;
}
void doamnhietdo()
{
  float t, h;
  am2315.readTemperatureAndHumidity(&t, &h);
  Serial.print("Nhiet do: ");
  Serial.println(t);
  Serial.print("Do am: ");
  Serial.println(h);
  Serial.print("\n");
  am1 = h;
  am2 = t;
}
void khongkhi()
{
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(am2.toInt(), am1.toInt());
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(am2.toInt(), am1.toInt());

  /*Serial.print("MQ135 RZero: ");
    Serial.print(rzero);
    Serial.print("\t Corrected RZero: ");
    Serial.print(correctedRZero);
    Serial.print("\t Resistance: ");
    Serial.print(resistance);*/
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.print("ppm\n");
  air1 = correctedPPM;
}
void anhsang()
{
  float value = lightMeter.readLightLevel();
  Serial.print("Anh Sang: ");
  Serial.println(value);
  anhsang1 = value;
}
void sentESP8266()
{
  data ="*"+tach+"#"+amdat1+"#"+air1+"#"+am1+"#"+am2+"#"+anhsang1+"#"+value[0]+"#"+value[1]+"#"+value[2]+"#"+value[3]+"#"+value[4]+"#"+value[5]+"%";
  Atmega328p.print(data);
  tach="0";
  Serial.println(data);
}
void receiveESP8266()
{
  while(Atmega328p.available()>0)
  {
  String dataPic = Atmega328p.readStringUntil('%');
  Serial.println(dataPic);
  if (dataPic.startsWith("*")) {
    dataPic = dataPic.substring(1, dataPic.length());
    dataPic.concat("#");
    for (int i = 0; i < 6; i++) {
      value[i] = dataPic.substring(0, dataPic.indexOf("#", 0));
      dataPic.remove(0, dataPic.indexOf("#", 0) + 1);
    }
  }
}
}
void ShowLCD()
{
  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.print(am2.toInt());
  lcd.print((char) 223);
  lcd.print("C");
  lcd.setCursor(6, 0);
  lcd.print(char(3));
  lcd.print(am1.toInt());
  lcd.print("%  ");
  lcd.setCursor(12, 0);
  lcd.print(char(4));
  lcd.print(amdat1.toInt());
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print(char(5));
  lcd.print(anhsang1.toInt());
  lcd.print("Lux    ");
  lcd.setCursor(9, 1);
  lcd.print(char(1));
  lcd.print(air1.toInt());
  lcd.print("ppm  ");
}
void loop()
{
  reset1();
  nhieu=0;
  receiveESP8266();
  doamdat();
  anhsang();
  doamnhietdo();
  khongkhi();
  ShowLCD();
  nut5();
  chedotudong();
  sentESP8266();
  //haman();
  delay(2500);
}
