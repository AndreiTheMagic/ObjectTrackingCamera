#include <Arduino.h>
//#include <Servo.h>

/*
Servo LRservo;
Servo UDservo;

String command;
int transcommand=0;

int lrpin = 5;
int udpin = 4;

int posV = 90;
int posH = 90;

int step = 1;

int pos = 0;
void setup() {
  LRservo.attach(lrpin);
  UDservo.attach(udpin);
  Serial.begin(9600);

  LRservo.write(posH);
  UDservo.write(posV);

  
}

void loop() {
  /*
  if(Serial.available()){
    command=Serial.readStringUntil('\n');
    command.trim();
    if(command=="left"){
      transcommand=1;
    }
    else if(command=="right"){
        transcommand=2;
    }
    else if(command=="up"){
      transcommand=3;
    }
    else if(command=="down"){
      transcommand=4;
    }
    else if(command=="home"){
      transcommand=5;
    }
  }

  switch(transcommand){
    case 2:
      if(posH>=step){
        posH-=step;
        LRservo.write(posH);}
      break;
    case 1:
      if(posH<=180-step){
        posH+=step;
        LRservo.write(posH);}
      break;
    case 4:
      if(posV>=step){
      posV-=step;
      UDservo.write(posV);}
      break;
    case 3:
    if(posV<=180-step){
      posV+=step;
      UDservo.write(posV);}
      break;
    case 5:
      LRservo.write(90);
      UDservo.write(90);
      break;
    default:
      delay(15);
  }
  delay(15);
}*/
