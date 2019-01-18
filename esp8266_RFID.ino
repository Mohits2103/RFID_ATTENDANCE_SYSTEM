
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "USERNAME";
char wifiPassword[] = "PASSWORD";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "CAYENNE USERNAME";
char password[] = "CAYENNE PASSWORD";
char clientID[] = "CAYYENE CLIENTID";

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define RST_PIN         5  //D1     // Configurable, see typical pin layout above
#define SS_PIN          15 //D8      // Configurable, see typical pin layout above


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;
String Card_1 = "CARD_1";  // NOTE USE small alphabets INSTEAD OF BIG (EX - IF CARD ID ID 0x8A7B6C5D TYPE IT AS "8a 7b 6c 5d")
String Card_2 = "CARD-2";
String Card_3 = "CARD_3";
String Card_4 = "CARD_4";
//String Card_N="CARD_N"; //add as many as you need.

unsigned long lastMillis = 0;

int a[4];
int i;

int EEaddress0 = 1;
int EEaddress2 = 4;
int EEaddress4 = 8;
int EEaddress6 = 12;/*
   Initialize.
*/
void setup() {
  Serial.begin(9600);
  EEPROM.begin(32);
  Serial.begin(9600);         // Initialize serial communications with the PC
  while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                // Init SPI bus
 mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  // mfrc522.PCD_Init();         // Init MFRC522 card
  a[0] = EEPROM.get(EEaddress0, a[0]);
  a[1] = EEPROM.get(EEaddress2, a[1]);
  a[2] = EEPROM.get(EEaddress4, a[2]);
  a[3] = EEPROM.get(EEaddress6, a[3]);
  a[4] = EEPROM.get(EEaddress6, a[4]);
  
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Serial.println("Connected");
    pinMode(D4, OUTPUT);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}

void check1() {
  a[0]++;
  
  Serial.println(a[0]);
  EEPROM.put(EEaddress0, a[0]);
  EEPROM.commit();
    digitalWrite(D4, LOW);
  delay(2000);
}
void check2() {
  a[1]++;
  Serial.println(a[1]);
  EEPROM.put(EEaddress2, a[1]);
  EEPROM.commit();
    digitalWrite(D4, LOW);
  delay(2000);
}
void check3() {
  a[2]++;
  Serial.println(a[2]);
  EEPROM.put(EEaddress4, a[2]);
  EEPROM.commit();
    digitalWrite(D4, LOW);
delay(2000);
}
void check4() {
  a[3]++;
  Serial.println(a[3]);
  EEPROM.put(EEaddress6, a[3]);
  EEPROM.commit();
  digitalWrite(D4, LOW);
  delay(2000);
}

void loop() {
    digitalWrite(D4, HIGH);
Cayenne.loop();

  //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Cayenne.virtualWrite(0, a[0]);
    Cayenne.virtualWrite(1, a[1]);
    Cayenne.virtualWrite(2, a[2]);
    Cayenne.virtualWrite(3, a[3]);
    //Some examples of other functions you can use to send data.
    //Cayenne.celsiusWrite(1, 22.0);
    //Cayenne.luxWrite(2, 700);
    //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
  }
  Serial.println("reading");
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
/*if (read_rfid >= "1") {       // USE THESE 3 LINES TO KNOW THE ID 1ST
    Serial.println(read_rfid);
    delay(2000);
    return;
  }*/
  if (read_rfid == Card_1) 
    check1();
    
  if (read_rfid == Card_2) 
    check2();
  
  if (read_rfid == Card_3) 
    check3();
  
  if (read_rfid == Card_4) 
    check4();
  

}
