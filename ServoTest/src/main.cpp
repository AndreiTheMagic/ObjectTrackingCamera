#include <Arduino.h>
#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(2); // Attach the servo to pin 9
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  int val = 0; // Initialize the servo angle variable
  for(;val<=180;val+=10) {
    myServo.write(val); // Move the servo to the specified angle
    delay(500); // Wait for 0.5 seconds
    Serial.print("Servo angle: ");
    Serial.println(val); // Print the current angle to the serial monitor
  }
  for(;val>=0;val-=10) {
    myServo.write(val); // Move the servo back to the specified angle
    delay(500); // Wait for 0.5 seconds
    Serial.print("Servo angle: ");
    Serial.println(val); // Print the current angle to the serial monitor
  }
 //myServo.write(90);
}
