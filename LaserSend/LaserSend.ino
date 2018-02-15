/*
Sending a message byte by byte using a timed protocol.

Information is encoded in the time the signal is either high or low.
Every byte has a 8 'up and downs' for each bit
   _ _         _ _ _ _
...   |_ _ _ _|       |_ _ = [1,0,0,1] (or is it [0,1,1,0]?)

TODO add a parity bit (req. bidirectional comms)
*/
#include <Keypad.h>   //use the Keypad libraries
#define SPEED (2500)

int incomingByte = 0;   // for incoming serial data
bool hasByteToSend = false;
char byteToSend = ' ';

////////
//
//  keypad code
// 
////////
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = 
{
  { 
    'A','B','C','D'      }
  ,
  { 
    'E','F','G','H'      }
  ,
  { 
    'I',' ','K','L'      }
  ,
  { 
    'M','N','O','\n'      }
};
// NB using pins 0 and 1 preclude the use of Serial.print (or any Serial for that matter)
byte rowPins[ROWS] = { 0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void readKey()
{
  int correct = 0;
  char customKey = customKeypad.getKey();//get the key value
  if(customKey)
  {
    //Serial.print("Entered Key: ");
    //Serial.println(customKey);
    byteToSend = customKey;
    hasByteToSend = true;
  }
}

/////////
//
// laser sending 
//
/////////

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.println("ready to send...");


}

// the loop function runs over and over again forever
void loop() {
  readKey();
  if (hasByteToSend) { //Serial.available() > 0) {
    incomingByte = byteToSend; //Serial.read();
    //Serial.println(incomingByte);
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
    hasByteToSend = false;
  }
}

void myDelay(int d) {
  //accurate >3milliseconds
  //delay(d);
  //accurate >3micros, <16000micros
  delayMicroseconds(d);
}

