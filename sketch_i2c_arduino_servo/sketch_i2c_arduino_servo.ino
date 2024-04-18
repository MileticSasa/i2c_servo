#include<Wire.h>
#include<string.h>
#include<Servo.h>

#define SERVO_PIN 6

#define MY_ADDRESS  0x68

Servo my_servo;
int position = 0; //start position

void setup() 
{
  Serial.begin(115200);
  Wire.begin(MY_ADDRESS);

  my_servo.attach(SERVO_PIN);
  my_servo.write(position);

  Wire.onReceive(receiveEvent);

  Serial.println(".............................");
}

void loop() 
{

}

void receiveEvent(int size)
{
  while(Wire.available())
  {
    char c = Wire.read();
    position = (int)c;
    my_servo.write(position);
    Serial.println(position);
    delay(50);
  }
}
