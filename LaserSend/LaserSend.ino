/*
Sending a message byte by byte using a timed protocol.

Information is encoded in the time the signal is either high or low.
Every byte has a 8 'up and downs' for each bit
   _ _         _ _ _ _
...   |_ _ _ _|       |_ _ = [1,0,0,1] (or is it [0,1,1,0]?)

TODO add a parity bit (req. bidirectional comms)
*/

int incomingByte = 0;   // for incoming serial data

#define SPEED (1500)

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
    /*byte bits[8] = {1, 0, 0, 0, 0, 1, 1, 0};
    incomingByte = 0;
    for (int j = 7; j >= 0; j--) {
      incomingByte  = (incomingByte * 2) + bits[j];
    }*/

    digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 0; i < 8; i++) {

      if (bitRead(incomingByte,i))
      {
        myDelay(1 * SPEED);
      }
      else
      {
        myDelay(4 * SPEED);
      }
      if (i % 2 == 0) {
        digitalWrite(LED_BUILTIN, LOW);
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }

    digitalWrite(LED_BUILTIN, LOW);
    myDelay(6 * SPEED);

  }
}

void myDelay(int d) {
  //accurate >3milliseconds
  //delay(d);
  //accurate >3micros, <16000micros
  delayMicroseconds(d);
}

