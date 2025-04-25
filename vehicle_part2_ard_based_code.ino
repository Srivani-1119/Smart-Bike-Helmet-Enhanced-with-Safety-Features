#include <SPI.h>            // SPI for RFID
#include <MFRC522.h>        // RFID Library
#include <LiquidCrystal.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

//#include "SoftwareSerial.h"
//#include "DFRobotDFPlayerMini.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
//static const uint8_t PIN_MP3_TX = 5; // Connects to module's RX 
//static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX 
//SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
//DFRobotDFPlayerMini player;

const int distanceThreshold = 50;
long distance;
long dist;
// RFID MFRC522 Pins
#define SS_PIN 10
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define TRIG_PIN 7
#define ECHO_PIN 6

int rec_pin = 9;
//bool rec_st = false;

int out_pin = 4;
//bool out_st = false;

const int btn_pin = 2;
//bool btn_st = false;

void setup() {
    Serial.begin(9600);
    
    // RFID Setup
    SPI.begin();
    mfrc522.PCD_Init();

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(rec_pin, INPUT);

    pinMode(btn_pin, INPUT);

     lcd.begin(16, 2);
     lcd.setCursor(0,0);
     lcd.print("SMART - VEHICLE");

     lcd.setCursor(0,1);
     lcd.print("      VJIT      ");
         
    // Output Pins
    pinMode(out_pin, OUTPUT);
    //pinMode(OUT2, OUTPUT);
    digitalWrite(out_pin, LOW);
    //digitalWrite(OUT2, LOW);
    digitalWrite(btn_pin, LOW);
    digitalWrite(rec_pin, LOW);

  //  softwareSerial.begin(9600);

  // Start communication with DFPlayer Mini
  /*if (player.begin(softwareSerial)) {
   Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(20);
    // Play the first MP3 file on the SD card
    player.play(1);
delay(2000);
    
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }*/
  
    delay(500);
}

long measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2;  // Convert to cm
    return distance;
}


void loop() {
    
  if(digitalRead(btn_pin) == HIGH){
    clearLCDLine(0,0);
    lcd.setCursor(0, 0);
    lcd.print("PLZ WEAR HELMET:");
    //player.play(2);
    clearLCDLine(0,1);
    delay(10000);
    Serial.println("PLZ WEAR HELMET:");
    int helmet_st;
    helmet_st = digitalRead(rec_pin);
    Serial.print("Helmet status:");
    Serial.print(helmet_st);
    // Check for RF signal
    if(helmet_st == HIGH)
    {
          clearLCDLine(0,0);
          lcd.setCursor(0,0);
          lcd.print("HELMET WEARED");
          
          Serial.println("Helmet weared");
          delay(1000);
          
           clearLCDLine(0,1);
           lcd.setCursor(0, 1);
           lcd.print("PLZ SCAN LICENSE");
     //      player.play(3);
           delay(1000);

           String tagID = scanRFID();
          if (tagID != "") {
                  clearLCDLine(0,1);
                  lcd.setCursor(0,1);
                  lcd.print("LICENSE SCANNED:");
                 
                  delay(1000);
                  long dist = verifylicense(tagID);
                  delay(500);

                  if (dist > distanceThreshold) {
            digitalWrite(out_pin, HIGH);
            Serial.println("VEHICLE STARTED:");
            delay(5000);
            digitalWrite(out_pin, LOW);
        } else {
            digitalWrite(out_pin, LOW);
            Serial.println("OBSTACLE OCCURS:");
        }
                           
                  
                  /*else
                  {
                    digitalWrite(out_pin, LOW);
                    Serial.println("Vehicle should stop:");
                    delay(100);
                  }*/
          }
          /*else{
            digitalWrite(out_pin, LOW);
            Serial.println("Vehicle should stop:");
          }*/
        }
        else {
          clearLCDLine(0,0);
          lcd.setCursor(0,0);
          lcd.print("NO HELMET");
          digitalWrite(out_pin, LOW);

          clearLCDLine(0,1);
          lcd.setCursor(0,1);
          lcd.print("PLZ DONT DRIVE");
          delay(1000);
          Serial.println("No HELMET Con...");
        }
           delay(1000);
    }
   else if(digitalRead(btn_pin) == LOW)
   {
     lcd.setCursor(0,0);
     lcd.print("SMART - VEHICLE");
     digitalWrite(out_pin, LOW);

     lcd.setCursor(0,1);
     lcd.print("      VJIT      ");
   }
}
 
 // Function to check RFID tag
String scanRFID() {
  /*clearLCDLine(0,1);
  lcd.setCursor(0,1);
  lcd.print("PLEASE SCAN...");
*/
  String tagID = "";
  while(true) {
    // Check if a new RFID card is present
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        tagID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        tagID += String(mfrc522.uid.uidByte[i], HEX);
      }
      tagID.toUpperCase();
      mfrc522.PICC_HaltA();  // Stop reading
      break;
    }
  }
  delay(2000);
  return tagID;
}

long verifylicense(String tagID) {
  if(tagID == "736AEEC4")
  {
    clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("VALID LICENSE..");

     distance = measureDistance();
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        /*if (distance > distanceThreshold) {
            digitalWrite(out_pin, HIGH);
            Serial.println("Distance is high: Setting OUTPUT_PIN HIGH");
        } else {
            digitalWrite(out_pin, LOW);
            Serial.println("Distance is low: Setting OUTPUT_PIN LOW");
        }*/
    delay(1000);
  }
  else
  {
    clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("IN-VALID LICENSE");
    distance = 0;
    delay(1000);
  }
  return distance;
}
 
void clearLCDLine(int p,int line)
{
for(int n = p; n < 20; n++) 
{ // 20 indicates symbols in line. For 2x16 LCD write - 16
lcd.setCursor(n,line);
lcd.print(" ");
}
lcd.setCursor(p,line); // set cursor in the beginning of deleted line
}
