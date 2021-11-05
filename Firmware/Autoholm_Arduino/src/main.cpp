#include <Arduino.h>
#define B1 A3
#define B2 A2

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(B1, INPUT); // sets the pin as output
  pinMode(B2, INPUT); // sets the pin as output
  pinMode(LED_BUILTIN, OUTPUT);
}

void pressButton(int relative_bearing)
{
  int pin = relative_bearing < 0 ? B1 : B2;
  int value = abs(relative_bearing) >= 10 ? HIGH : LOW;
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pin, value);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
  delay(250);
  pinMode(pin, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print(pin);
  Serial.print(" ");
  Serial.println(value);
  delay(100);                                                                             
}

void updateBearing(int relative_bearing)
{
  int direction = relative_bearing / abs(relative_bearing);
  while (relative_bearing != 0)
  {
    pressButton(relative_bearing);
    if (abs(relative_bearing) >= 10)
    {
      relative_bearing -= direction * 10;
    }
    else
    {
      relative_bearing -= direction;
    }
  }
}

void loop()
{
  // reply only when you receive data:
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    String s = Serial.readString();
    int i = s.toInt();
    if (abs(i) >= 180)
      return;
    updateBearing(i);
    Serial.print("Recevied: ");
    Serial.println(i);
  }
}