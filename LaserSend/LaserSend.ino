/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  pin
  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 2 Sep 2016
  by Arturo Guadalupi

  modified 8 Sep 2016
  by Colby Newman
*/

#define SPEED (40)

int incomingByte = 0;   // for incoming serial data

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("ready to send...");


}

// the loop function runs over and over again forever
void loop() {

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    //flash();
    //flash();

    for (int i = 0; i < 8; i++) {
      
      digitalWrite(LED_BUILTIN, HIGH);
      myDelay(1 * SPEED);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print(i);
      Serial.print(" ");
      Serial.println(incomingByte % 2);
      if (incomingByte % 2 == 1)
      {
        myDelay(2.5 * SPEED);
      }
      else
      {
        myDelay(4 * SPEED);
      }
      // if (i<7){
      incomingByte = incomingByte / 2;
      //}

    }

    digitalWrite(LED_BUILTIN, HIGH);
    myDelay(1 * SPEED);
    digitalWrite(LED_BUILTIN, LOW);

    myDelay(6 * SPEED);
  }
}

void myDelay(int d) {
  //accurate >3milliseconds
  delay(d);
  //accurate >3micros, <16000micros
  //delayMicroseconds(d);
}

