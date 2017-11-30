const byte ledPin = 13;
const byte interruptPin = 2;
volatile int state = 0;

unsigned long lastTime = 0;

int readByte = 0;

int newByte = 0;

byte bits[8] = {0,0,0,0,0,0,0,0};
int i = 0;

#define SPEED (10)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  Serial.println("read to receive...");
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  if (newByte != 0) 
    {
      Serial.print((char)readByte);
      newByte = 0;
    }
}

void blink() {
  unsigned long curr = millis();
  int value = digitalRead(interruptPin);
  if (curr-lastTime <= ((0.5)*SPEED) || curr-lastTime > (7*SPEED)) {
    state = 0;
  }

   /*Serial.print("!\n");

   Serial.print(curr-lastTime); 
 Serial.print(" ");
   
    Serial.print(state);
    Serial.print(" ");
    Serial.print(value);
    Serial.print(" ");
    Serial.print(i);*/
    
  switch (state) {
    case 0: state = (value==0)?1:0; i=0;  break;
    case 1: state = (value==1)?2:0; break;
    default: 
      if ((curr-lastTime) < ((3*SPEED))) bits[i] = 1;
      else bits[i] = 0;

      if (i == 7) {
        readByte = 0;
        for (int j = 7; j >= 0; j--){
          readByte = (readByte * 2) + bits[j];
        }
        newByte = 1;
        state = 0;
      }
      else {
        i++;
        state=1;
      }
      
    break;
   }
   lastTime = curr;
}
