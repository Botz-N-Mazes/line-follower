#define TRIG_PIN 9   // Pin connected to TRIG
#define ECHO_PIN 10  // Pin connected to ECHO
#define SOUND_SPEED 343.0  // Speed of sound in air in m/s (343 m/s)

long duration;  // To store the time ECHO pin remains HIGH
float distance; // To store the calculated distance in cm

void setup() {
  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG as an OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Set ECHO as an INPUT
  Serial.begin(9600);         // Initialize serial communication for debugging
}

void loop() {
  // Step 1: Trigger the sensor
  digitalWrite(TRIG_PIN, LOW);  // Ensure TRIG is LOW
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); // Set TRIG to HIGH
  delayMicroseconds(10);        // Send a 10us pulse
  digitalWrite(TRIG_PIN, LOW);  // Reset TRIG to LOW

  // Step 2 & 3: Measure the duration ECHO pin is HIGH
  duration = pulseIn(ECHO_PIN, HIGH);

  // Step 4: Calculate distance (Equation: distance = (time * speed) / 2)
  distance = (duration * 0.0343) / 2; // Speed of sound = 343 m/s -> 0.0343 cm/us

  // Step 5: Display the result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Wait before next measurement
}
