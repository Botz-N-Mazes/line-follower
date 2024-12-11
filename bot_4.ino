// Motor control pins (L298N)
#define ENA 3    // Speed control for Left Motor (PWM)
#define IN1 5    // Direction control for Left Motor
#define IN2 6    // Direction control for Left Motor

#define ENB 11   // Speed control for Right Motor (PWM)
#define IN3 9    // Direction control for Right Motor
#define IN4 10   // Direction control for Right Motor

// Sensor pins
const int sensorPins[6] = {A0, A1, A2, A3, A4, A5}; // 6 sensor inputs
const int maxReadingCount = 400; // Increased readings per sensor
int sensorReadings[maxReadingCount]; // Storage for readings of a single sensor
float thresholds[6] = {513.15, 323.51, 331.62, 357.65, 430.59, 432.5}; // Thresholds for each sensor
// float thresholds[6] = {491.13, 351.56, 437.47, 449.24, 468.8, 503.89}; // Thresholds for each sensor
int weights[6] = {-3, -2, -1, 1, 2, 3}; // Adjust weights based on sensor layout

// Motor speed settings
int baseSpeed = 130; // Base motor speed (adjust as needed)
int correctionFactor = 50; // Correction factor for steering adjustments
int leftMotorErrorCorrection = 22;
int rightMotorErrorCorrection = 0; 

void setup() {
  // Initialize motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize sensor pins as inputs
  for (int i = 0; i < 6; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  Serial.begin(9600);
  Serial.println("Starting Sensor Calibration...");

  // Perform sequential calibration for all sensors
  // for (int sensorIndex = 0; sensorIndex < 6; sensorIndex++) {
  //   Serial.print("Calibrating Sensor ");
  //   Serial.println(sensorIndex);
  //   calibrateSensor(sensorIndex, thresholds);
  //   delay(1000); // Delay between calibrations for stability
  // }

  Serial.println("Calibration complete!");
  Serial.println("Thresholds for all sensors:");
  for (int i = 0; i < 6; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": Threshold = ");
    Serial.println(thresholds[i]);
  }

  Serial.println("Line Following Robot Ready.");
}

void loop() {
  int sensorBinary[6]; // Store binary states of sensors
  int positionSum = 0;
  int activeSensors = 0;

  // Step 1: Read sensor values and apply thresholds
  for (int i = 0; i < 6; i++) {
    int rawValue = analogRead(sensorPins[i]); // Read sensor value
    sensorBinary[i] = (rawValue > thresholds[i]) ? 1 : 0; // Apply threshold
    positionSum += sensorBinary[i] * weights[i]; // Calculate weighted position
    activeSensors += sensorBinary[i]; // Count active sensors
  }
  // while (activeSensors == 0) {
  //   setMotorSpeeds(0, 0);
  //   Serial.println("No sensors active");
  //   delay(100);
  // }
  // Step 2: Calculate error
  float error = (activeSensors > 0) ? positionSum / float(activeSensors) : 0;

  // Step 3: Determine motor speeds based on error
  int leftMotorSpeed = baseSpeed - (error * correctionFactor);
  int rightMotorSpeed = baseSpeed + (error * correctionFactor);

  // Constrain motor speeds to valid range
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  // Set motor speeds
  setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

  // Visualize sensor states
  visualizeSensors(sensorBinary, 6);

  // delay(100); // Add a small delay to avoid flooding the Serial Monitor
}

// Function to set motor speeds
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Control left motor
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, leftSpeed + leftMotorErrorCorrection);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -leftSpeed + leftMotorErrorCorrection);
  }

  // Control right motor
  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, rightSpeed + rightMotorErrorCorrection);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -rightSpeed + rightMotorErrorCorrection);
  }
}

// Function to visualize sensor states
void visualizeSensors(int sensorBinary[], int numSensors) {
  Serial.print("Sensors: ");
  for (int i = 0; i < numSensors; i++) {
    if (sensorBinary[i] == 1) {
      Serial.print("|||"); // Active sensor
    } else {
      Serial.print("__"); // Inactive sensor
    }
  }
  Serial.println(); // New line for the next reading
}

// Function to calibrate a single sensor
void calibrateSensor(int sensorIndex, float thresholds[]) {
  Serial.println("Collecting sensor readings...");
  setMotorSpeeds(0, 0);
  int readingCounts = 0;

  // Collect readings for the current sensor
  while (readingCounts < maxReadingCount) {
    // Read sensor value
    sensorReadings[readingCounts] = analogRead(sensorPins[sensorIndex]);
    readingCounts++;
    // Increment the count and reset after a full cycle
  }
  
  Serial.println("Max of readings reached");

  // Calculate min and max for the current sensor
  int maxValue = sensorReadings[0];
  int minValue = sensorReadings[0];
  for (int j = 1; j < maxReadingCount; j++) {
    if (sensorReadings[j] > maxValue) {
      maxValue = sensorReadings[j];
    }
    if (sensorReadings[j] < minValue) {
      minValue = sensorReadings[j];
    }
  }

  Serial.print("Sensor ");
  Serial.print(sensorIndex);
  Serial.print(": Min = ");
  Serial.print(minValue);
  Serial.print(", Max = ");
  Serial.println(maxValue);

  // Perform K-means clustering to determine centroids
  float centroids[2] = {0, 0};
  calculateCentroids(sensorReadings, maxReadingCount, centroids);
  thresholds[sensorIndex] = (centroids[0] + centroids[1]) / 2; // Set threshold as midpoint of centroids

  Serial.print("Sensor ");
  Serial.print(sensorIndex);
  Serial.print(": Centroid 1 = ");
  if (centroids[0] < centroids[1]) {
    Serial.print(centroids[0]);
  } else {
    Serial.print(centroids[1]);
  }
  Serial.print(", Centroid 2 = ");
  if (centroids[0] > centroids[1]) {
    Serial.print(centroids[0]);
  } else {
    Serial.print(centroids[1]);
  }
  Serial.println();
  Serial.print("Threshold = ");
  Serial.println(thresholds[sensorIndex]);
}

// Function to calculate centroids for a single sensor
void calculateCentroids(int readings[], int size, float centroids[]) {
  int counts[2] = {0, 0}; // Count of values assigned to each cluster
  centroids[0] = readings[0]; // Initialize first centroid
  centroids[1] = readings[size - 1]; // Initialize second centroid
  Serial.print("Initial Centroids: ");
  Serial.print(centroids[0]);
  Serial.print(", ");
  Serial.println(centroids[1]);

  // Perform K-means clustering (3 iterations)
  for (int iter = 0; iter < 3; iter++) {
    float sum[2] = {0, 0};
    counts[0] = 0; // Reset counts for each iteration
    counts[1] = 0;

    for (int i = 0; i < size; i++) {
      int cluster = (abs(readings[i] - centroids[0]) < abs(readings[i] - centroids[1])) ? 0 : 1;
      sum[cluster] += readings[i];
      counts[cluster]++;
    }
    for (int j = 0; j < 2; j++) {
      if (counts[j] > 0) centroids[j] = sum[j] / counts[j];
    }
  }

  // Print updated centroids and counts
  Serial.print("Updated Centroids: ");
  Serial.print(centroids[0]);
  Serial.print(", ");
  Serial.println(centroids[1]);

  // Final cluster counts
  Serial.print("Final cluster counts: ");
  Serial.print(counts[0]);
  Serial.print(", ");
  Serial.println(counts[1]);
}
