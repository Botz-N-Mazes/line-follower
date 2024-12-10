const int sensorPins[6] = {A0, A1, A2, A3, A4, A5}; // Sensor pins
const int maxReadingCount = 130; // Maximum readings per sensor
int sensorReadings[6][maxReadingCount]; // Storage for sensor readings
int readingCounts = 0; // Number of total readings stored (shared across all sensors)
float centroids[6][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Sensor Clustering...");
}

void loop() {
  // Step 1: Collect readings until the array is full
  Serial.println("Collecting sensor readings...");
  delay(2000);
  readingCounts = 0; // Reset reading count for each cycle

  while (readingCounts < maxReadingCount) {

    for (int i = 0; i < 6; i++) {
      if (readingCounts < maxReadingCount) {
        sensorReadings[i][readingCounts] = analogRead(sensorPins[i]);
      }
    }
    readingCounts++; // Increment after collecting from all sensors
    delay(50);
  }
  Serial.println("Max of readings reached");
  for (int i = 0; i < 6; i++) {
    int maxValue = sensorReadings[i][0];
    int minValue = sensorReadings[i][0];
    for (int j = 1; j < maxReadingCount; j++) {
        if (sensorReadings[i][j] > maxValue) {
            maxValue = sensorReadings[i][j];
        }
        if (sensorReadings[i][j] < minValue) {
            minValue = sensorReadings[i][j];
        }
    }
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(maxValue);
    Serial.print(", ");
    Serial.println(minValue);
}
  
  // Step 2: Perform K-means clustering for each sensor
  Serial.println("Calculating centroids...");
  for (int m = 0; m < 6; m++) {
    Serial.print("Sensor ");
    Serial.println(m);
    calculateCentroids(sensorReadings[m], maxReadingCount, centroids[m]);
    delay(1000);
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

  //Print updated centroids and counts for the current iteration
  Serial.print("Updated Centroids: ");
  Serial.print(centroids[0]);
  Serial.print(", ");
  Serial.println(centroids[1]);
  delay(500);

  // Final cluster counts
  Serial.print("Final cluster counts: ");
  Serial.print(counts[0]);
  Serial.print(", ");
  Serial.println(counts[1]);
  delay(500);

}
