#include<ESP8266WiFi.h>

#define GREEN_LED  4  // GPIO4 (D2)
#define RED_LED    0  // GPIO0 (D3)

#define LED 5 // (D1)

const char* ssid = "OPPO F23 5G";
const char* password = "123456789";

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(LED, OUTPUT);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
        }

    Serial.println("Connected to WiFi!");
    Serial.print("Server IP: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();
  
    if (client) {
        Serial.println("Client Connected!");
        while (client.connected()) {
            if (client.available()) {
                  String data = client.readStringUntil('\n'); // Read received data
                  data.trim(); // Remove extra spaces/newlines
                  Serial.print("Received: ");
                  Serial.println(data);

        // You can use the received value for further processing
        
        if (data == "1") {  // Button pressed
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(LED, HIGH);
        digitalWrite(RED_LED, LOW);
    }  else if (data == "0") {  // Button released
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(LED, LOW);
        digitalWrite(RED_LED, HIGH);
    }
        
        client.println("ACK: " + data); // Send acknowledgment
      }
    }
    Serial.println("Client Disconnected");
    client.stop();  // Close connection properly
  }
}
