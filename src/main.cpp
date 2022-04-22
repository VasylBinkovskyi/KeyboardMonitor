#include <Arduino.h>
#include <KeyMonitor.h>

int AnalogPin = A0;
int DigitalPin = 8;

void setup()
{
  Serial.begin(9600);
  pinMode(AnalogPin, INPUT);
  pinMode(DigitalPin, INPUT);
}

void loop()
{
  int analogKeyResult = AnalogKeyMonitor(AnalogPin);
  int digitalKeyResylt = DigitalKeyMonitor(DigitalPin);

  switch (analogKeyResult)
  {
  case 11:
    Serial.println("A_Button1: press");
    break;
  case 12:
    Serial.println("A_Button2: press");
    break;
  case 13:
    Serial.println("A_Button3: press");
    break;
  case 21:
    Serial.println("A_Button1: Short press");
    break;
  case 22:
    Serial.println("A_Button2: Short press");
    break;
  case 23:
    Serial.println("A_Button3: Short press");
    break;
  case 31:
    Serial.println("A_Button1: Long Press");
    break;
  case 32:
    Serial.println("A_Button2: Long Press");
    break;
  case 33:
    Serial.println("A_Button3: Long Press");
    break;
  case 41:
    Serial.println("A_Button1: Long Pressed");
    break;
  case 42:
    Serial.println("A_Button2: Long Pressed");
    break;
  case 43:
    Serial.println("A_Button3: Long Pressed");
    break;
  case 51:
    Serial.println("A_1and2: Long Press");
    break;
  case 52:
    Serial.println("A_1and3: Long Press");
    break;
  case 53:
    Serial.println("A_2and3: Long Press");
    break;
  }

  switch (digitalKeyResylt)
  {
  case 1:
    Serial.println("D_Button: press");
    break;
  case 2:
    Serial.println("D_Button: short Press");
    break;
  case 3:
    Serial.println("D_Button: Long Press");
    break;
  case 4:
    Serial.println("D_Button: Long Pressed");
    break;
  }
}