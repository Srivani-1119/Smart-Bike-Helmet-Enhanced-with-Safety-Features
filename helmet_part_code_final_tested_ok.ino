#include <ESP8266WiFi.h>

const char* ssid = "OPPO F23 5G";
const char* password = "123456789";
const char* serverIP = "192.168.106.220"; // Change this to your server's IP
const uint16_t port = 80;

WiFiClient client;
const int buttonPin = 5; // GPIO5 (D1)
#define MQ135_PIN A0  // Analog pin for sensor

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Internal pull-up resistor
   pinMode(MQ135_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected to WiFi!");
}

void loop() {

  int sensorValue = analogRead(MQ135_PIN);  // Read sensor value
    float voltage = sensorValue * (3.3 / 1023.0); // Convert to voltage
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    delay(1000);

    int buttonState = digitalRead(buttonPin); // Read button

    int dataToSend;
    
  if (!client.connected()) {
    Serial.println("Reconnecting to server...");
    if (client.connect(serverIP, port)) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Connection failed!");
      delay(3000);
      return;
    }
  }

  
  if(buttonState == HIGH && sensorValue > 200)
  {
    dataToSend = 1;
  }
  else if(buttonState == HIGH && sensorValue < 200)
  {
    dataToSend = 0;
  }
  else if(buttonState == LOW && sensorValue > 300)
  {
    dataToSend = 0;
  }
  else if(buttonState == LOW && sensorValue < 300)
  {
    dataToSend = 0;
  }
  
  Serial.print("Sending: ");
  Serial.println(dataToSend);
  client.println(dataToSend); // Send 1 or 0

  delay(2000); // Send every 5 seconds
}
