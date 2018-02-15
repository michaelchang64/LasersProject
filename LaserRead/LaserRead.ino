/*
Receiving a message based on a timed protocol described in LaserSend.


*/

const byte ledPin = 13;
const byte interruptPin = 2;
volatile int state = 0;

unsigned long lastTime = 0;

int readByte = 0;

int newByte = 0;

byte bits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int i = 0;

#define SPEED (1000)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  Serial.println("read to receive......");
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  if (newByte != 0)
  {
    //Serial.print(readByte);
    Serial.print((char)readByte);
    newByte = 0;
  }
}

//check that the change was real, and not 'hardware bounce'
//https://arduino.stackexchange.com/a/13301
int oldValue = 1; //high means laser is off

void blink() {
  //unsigned long curr = millis();
  unsigned long curr = micros();
  int value = digitalRead(interruptPin);
  if (oldValue == value) {
    return;
  }
  else {
    oldValue = value;
  }

  if (curr - lastTime <= ((1)*SPEED) || curr - lastTime > (5 * SPEED)) {
    i = 0;
    //Serial.print(curr-lastTime);
    //Serial.println(" RESET");
  }

    /*Serial.print("\n");
    Serial.print(" ");
    Serial.println(curr-lastTime);
    Serial.print(" ");

    //Serial.print(state);
    //Serial.print(" ");
    Serial.print(value);
    Serial.print(" ");
    Serial.print(i);
    Serial.println(" ");*/

  switch (i) {
    //i 0 = start of byte
    case 0: i = 1;  break;
    //i 1 = end of byte
    case 7:
        readByte = 0;
        for (int j = 7; j >= 0; j--) {
          readByte = (readByte * 2) + bits[j];
        }
        newByte = 1;
        i = 0;
        break;
    default:
      getBit((i-1), curr - lastTime);
      i++;
      break;
  }
  lastTime = curr;
}

void getBit(int i, int t) {
  if (t < ((3 * SPEED))) bits[i] = 1;
  else bits[i] = 0;
  //      Serial.print(t);
  //      Serial.print(" ");
  //      Serial.println(bits[i]);
}

