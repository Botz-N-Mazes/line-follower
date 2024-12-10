const int sensorPin = A0; // Current sensor pin (will switch between A0 to A5)
const int maxReadingCount = 500; // Increased readings per sensor (6 times)
int sensorReadings[maxReadingCount]; // Storage for readings of a single sensor
float centroids[2] = {0, 0}; // Centroids for the current sensor

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Sensor Clustering...");
}

void loop() {
  for (int sensorIndex = 0; sensorIndex < 6; sensorIndex++) {
    Serial.print("Calibrating Sensor ");
    Serial.println(sensorIndex);

    // Step 1: Collect readings for the current sensor
    Serial.println("Collecting sensor readings...");
    delay(2000);
    int readingCounts = 0; // Reset reading count for the current sensor

    while (readingCounts < maxReadingCount) {
      sensorReadings[readingCounts] = analogRead(sensorIndex + A0); // Read from current sensor
      readingCounts++; // Increment after collecting a reading
      delay(50); // Delay for stability
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
    Serial.print(": Max = ");
    Serial.print(maxValue);
    Serial.print(", Min = ");
    Serial.println(minValue);

    // Step 2: Perform K-means clustering for the current sensor
    Serial.println("Calculating centroids...");
    calculateCentroids(sensorReadings, maxReadingCount, centroids);

    // Print centroids for the current sensor
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

    delay(3000); // Wait before calibrating the next sensor
  }

  Serial.println("All sensors calibrated. Restarting...");
  delay(10000); // Wait before starting fresh
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
  // delay(500);

  // Final cluster counts
  Serial.print("Final cluster counts: ");
  Serial.print(counts[0]);
  Serial.print(", ");
  Serial.println(counts[1]);
  // delay(500);
}
