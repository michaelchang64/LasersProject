//twitter token: 2413286622-TKO6va6Z3Q66X96W570BhibHE6U2mfL25TZHlJA
//#if defined(ARDUINO) && ARDUINO > 18   // Arduino 0019 or later
#include <SPI.h>
#endif
#include <WiFi.h>
#include <Twitter.h>

#define SPEED (40)  //Speed at which bytes are being read in
#define TOKEN "2413286622-TKO6va6Z3Q66X96W570BhibHE6U2mfL25TZHlJA" // OAuth Key
#define LIB_DOMAIN "arduino-tweet.appspot.com" // Twitter Proxy

const byte ledPin = 13;       // pin for LED
const byte interruptPin = 2;  // pin for
volatile int state = 0;
unsigned long lastTime = 0;   // Duration of time during which nothing is being sent
int buttonState = 0;          // Button state when pressed uploads tweet
int readByte = 0;             // Reading in a byte
int newByte = 0;              // New byte

byte bits[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Take in array of bytes, parse into char later
int i = 0;

int count = 0;      // Counter
char *msg = calloc(140, sizeof(char));   // Read in bytes then store in char array

char ssid[] = "YaleGuest";
int status = WL_IDLE_STATUS;
Twitter twitter(TOKEN);

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  Serial.println("ready to receive...");
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network.");
  printCurrentNet();
  printWifiData();
}

void loop() {
  if (buttonState == HIGH) {    // Sends tweet if button is pressed
    if (twitter.post(msg)) {
      int status = twitter.wait();
      if (status == 200) {
        Serial.println("OK.");
        reInitArray(msg);
        count = 0;
      } else {
        Serial.print("failed upload, error ");
        Serial.println(status);
      }
    } else {
      Serial.println("connection failed.");
    }
    do {} while (1);
  } else if (newByte != 0) {    //receives messages, adds to char array msg
    msg[count] = (char)readByte;
    //Serial.print(readByte);
    Serial.print((char)readByte);
    newByte = 0;
    count++;
  }
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void reinitArray(char *arr) {
  int j = 0;
  while(j < 140) {
    arr[j] = 0;
  }
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
}
