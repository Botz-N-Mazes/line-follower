// Define the analog pins connected to the sensor array
#define NUM_SENSORS 6
const int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5};

// Threshold for detection (adjust as needed)
int detectionThreshold = 500;

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  Serial.println("TCRT5000 8-Channel Sensor Array - Advanced Test");
  Serial.println("Move an object or line over the sensors to observe detection...");
}

void loop() {
  int sensorValues[NUM_SENSORS];
  bool sensorStates[NUM_SENSORS];

  // Read sensor values
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    // Determine if the sensor is detecting a surface based on the threshold
    sensorStates[i] = (sensorValues[i] > detectionThreshold);
  }

  // Print raw sensor values
  Serial.print("Raw Values: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Print detection states
  Serial.print("Detection: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorStates[i]) {
      Serial.print("[#] "); // Detected
    } else {
      Serial.print("[ ] "); // Not detected
    }
  }
  Serial.println();

  // Visualize sensor values
  visualizeSensors(sensorValues, detectionThreshold);

  // Small delay for better readability in Serial Monitor
  delay(1500);
}

// Function to visualize sensor values in a bar chart-like form
void visualizeSensors(int values[], int threshold) {
  Serial.println("Sensor Visualization:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    int barLength = map(values[i], 0, 1023, 0, 20); // Map sensor value to bar length
    for (int j = 0; j < barLength; j++) {
      Serial.print("|");
    }
    Serial.print(" (");
    Serial.print(values[i]);
    Serial.println(")");
  }
  Serial.println("---------------------------");
}
