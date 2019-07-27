#include <SoftwareSerial.h>  //BT
#include <Servo.h>  //SERVO
#include <GPD2846.h>  //MP3
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
SoftwareSerial hc(0,1);
Servo servo;
//Led Matrix 
#include "Parola_Fonts_data.h"
const uint16_t WAIT_TIME = 1000;
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
int pos = 0;
String msg;
String dst;
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//Motor 1
const int in4 = 6; //L motor back
const int in3 = 7; //L motor FORWOARD
//Motor 2
const int in2 = 8; //R motor back
const int in1 = 9; //R motor forwoard
String data = "";

//Ultrasonic 
const int trigPin = 4;
const int echoPin = 2;
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  hc.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(5, OUTPUT); //mp3 start
  pinMode(12, OUTPUT); //mp3 enable
  digitalWrite(12, HIGH); //mp3 enable
  servo.attach(3);
  P.begin();
  P.setFont(fontHeb);
  P.setTextEffect(PA_SCROLL_LEFT,PA_SCROLL_LEFT);
  P.print("Trafh");
  //Serial.println("Welcome to Trafh");
}
//Movement
  void moveStop()
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  void moveF()
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  void moveB()
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  void moveR()
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  void moveL()
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

  }
void loop() {
  // put your main code here, to run repeatedly:
    while(hc.available()>0) {
    data+=(char)hc.read();
    }
    if(data != "")
    {
    Serial.print("Data Recieved: ");
    Serial.println(data);
      if(data == "0") {
      moveStop();
      Serial.println("Stop");
      }
      else if(data == "8") {
      moveF();
      Serial.println("Forwoard");
      }
      else if(data == "2") {
      moveB();
      Serial.println("Back");
      }
      else if(data == "4") {
      moveL();
      Serial.println("Left");
      }
      else if(data == "6") {
      moveR();
      Serial.println("Right");
      }
      
      //Music Contorll
      else if(data == "M") {
      digitalWrite(5, HIGH);
      Serial.println("Play");
      }
      else if(data == "N") {
      digitalWrite(5, LOW);
      Serial.println("Pause");
      }
      
      //Servo Contorl
      else if(data == "O") 
      {
           servo.write(100); 
           delay(180);
           servo.write(90);
           Serial.println("Open");
      }
      
      else if(data == "C") {
       servo.write(0); 
       delay(150);
       servo.write(90);
       Serial.println("Close");
      }
      
      // Distance Display
      else if(data == "D") {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance= duration*0.034/2;
      Serial.print("Distance: ");
      Serial.println(distance);
      dst = String(distance + " [CM]");
      P.print(distance);
      }
      else if(data.length()>3)
      P.print(data);
  }
  data = "";
}
