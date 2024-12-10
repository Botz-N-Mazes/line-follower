const int sensorPins[6] = {A0, A1, A2, A3, A4, A5}; // Sensor pins
const int maxReadingCount = 120; // Maximum readings per sensor
int sensorReadings[6][maxReadingCount]; // Storage for sensor readings
int readingCounts = 0; // Number of total readings stored (shared across all sensors)
float centroids[6][2]; // Centroids for each sensor

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Sensor Clustering...");
}

void loop() {
  // Step 1: Collect readings until the array is full
  Serial.println("Collecting sensor readings...");
  readingCounts = 0; // Reset reading count for each cycle

  while (readingCounts < maxReadingCount) {
    for (int i = 0; i < 6; i++) {
      if (readingCounts < maxReadingCount) {
        sensorReadings[i][readingCounts] = analogRead(sensorPins[i]);
        Serial.print(sensorReadings[i][readingCounts]);
        Serial.print(" ");
      }
      Serial.println();
    }
    readingCounts++; // Increment after collecting from all sensors
    // if (readingCounts % 40 == 0) {
    //   Serial.println(readingCounts); // Print progress
    // }
    delay(100); // Wait for the next reading (sampling every 100 ms)
  }

  // Step 2: Perform K-means clustering for each sensor
  Serial.println("Calculating centroids...");
  for (int i = 0; i < 6; i++) {
    calculateCentroids(sensorReadings[i], maxReadingCount, centroids[i]);
  }

  // Step 3: Print centroids
  Serial.println("Centroids:");
  for (int i = 0; i < 6; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": Centroid 1 = ");
    if (centroids[i][0] < centroids[i][1]) {
      Serial.print(centroids[i][0]);
    } else {
      Serial.print(centroids[i][1]);
    }
    Serial.print(", Centroid 2 = ");
    if (centroids[i][0] > centroids[i][1]) {
      Serial.print(centroids[i][0]);
    } else {
      Serial.print(centroids[i][1]);
    }
    Serial.println();
  }

  delay(1000); // Wait before starting fresh
}

// Function to calculate centroids for a single sensor
void calculateCentroids(int readings[], int size, float centroids[]) {
  int counts[2] = {0, 0}; // Count of values assigned to each cluster
  centroids[0] = readings[0]; // Initialize first centroid
  centroids[1] = readings[size - 1]; // Initialize second centroid

  // Perform K-means clustering (3 iterations)
  for (int iter = 0; iter < 3; iter++) {
    float sum[2] = {0, 0};
    for (int i = 0; i < size; i++) {
      int cluster = (abs(readings[i] - centroids[0]) < abs(readings[i] - centroids[1])) ? 0 : 1;
      sum[cluster] += readings[i];
      counts[cluster]++;
    }
    for (int j = 0; j < 2; j++) {
      if (counts[j] > 0) centroids[j] = sum[j] / counts[j];
    }
  }
  Serial.println("Count of values assigned to each cluster:");
  Serial.print("Cluster 0: ");
  Serial.println(counts[0]);
  Serial.print("Cluster 1: ");
  Serial.println(counts[1]);
  Serial.println(counts[0] + counts[1]);
}