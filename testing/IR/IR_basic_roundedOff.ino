// Define the analog pins connected to the sensors
const int sensorPins[6] = {A0, A1, A2, A3, A4, A5};

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  Serial.println("Reading and rounding sensor values...");
}

void loop() {
  int sensorValues[6];

  // Read and round sensor values
  for (int i = 0; i < 6; i++) {
    int rawValue = analogRead(sensorPins[i]);        // Read raw sensor value
    sensorValues[i] = ((rawValue + 50) / 100) * 100; // Round to the nearest 100
  }

  // Print the rounded sensor values
  Serial.print("Rounded Values: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println(); // New line for each reading

  delay(100); // Small delay for readability
}
