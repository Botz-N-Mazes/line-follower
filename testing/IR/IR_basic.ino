// Define the analog pins connected to the sensor array
#define SENSOR_0 A0
#define SENSOR_1 A1
#define SENSOR_2 A2
#define SENSOR_3 A3
#define SENSOR_4 A4
#define SENSOR_5 A5
#define SENSOR_6 A6
#define SENSOR_7 A7

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Optional: Print a startup message
  Serial.println("TCRT5000 8-Channel Sensor Array Test Initialized");
}

void loop() {
  // Read sensor values
  int sensorValues[8];
  sensorValues[0] = analogRead(SENSOR_0);
  sensorValues[1] = analogRead(SENSOR_1);
  sensorValues[2] = analogRead(SENSOR_2);
  sensorValues[3] = analogRead(SENSOR_3);
  sensorValues[4] = analogRead(SENSOR_4);
  sensorValues[5] = analogRead(SENSOR_5);
  sensorValues[6] = analogRead(SENSOR_6);
  sensorValues[7] = analogRead(SENSOR_7);

  // Print sensor values to the Serial Monitor
  Serial.print("Sensor values: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println(); // New line after all sensor values

  // Optional: Add a delay for easier reading in the Serial Monitor
  delay(500);
}
