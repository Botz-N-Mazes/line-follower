#include <SoftwareSerial.h>

// Define RX and TX pins for Bluetooth module
SoftwareSerial BTSerial(10, 11); // RX, TX
const int BW = 9600;

void setup() {
  Serial.begin(BW);       // For Serial Monitor
  for (int i = 0; i < 10; i++) {
    Serial.println();
  }

  BTSerial.begin(BW);     // Bluetooth default baud rate in AT mode
  Serial.println("Bluetooth in AT command mode. Type AT commands below:");
}

void loop() {
  // Send data from Serial Monitor to Bluetooth module
  if (Serial.available()) {
    char command[50];
    int index = 0;
    while (Serial.available() && index < sizeof(command) - 1) {
      command[index++] = Serial.read();
    }
    command[index] = '\0'; // Null-terminate the string
    BTSerial.print(command); // Send command to Bluetooth module
    BTSerial.print("\r\n"); // Append termination if required
    Serial.print("Sent: ");
    Serial.println(command);
  }

  // Receive data from Bluetooth module and display on Serial Monitor
  if (BTSerial.available()) {
    char response[50];
    int index = 0;
    while (BTSerial.available() && index < sizeof(response) - 1) {
      response[index++] = BTSerial.read();
    }
    response[index] = '\0'; // Null-terminate the string
    Serial.print("Received: ");
    Serial.println(response);
  }
}
