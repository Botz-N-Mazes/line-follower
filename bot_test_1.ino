// Motor control pins (L298N)
#define ENA 3    // Speed control for Left Motor (PWM)
#define IN1 5    // Direction control for Left Motor
#define IN2 6    // Direction control for Left Motor

#define ENB 11   // Speed control for Right Motor (PWM)
#define IN3 9    // Direction control for Right Motor
#define IN4 10   // Direction control for Right Motor

// Sensor pins
const int sensorPins[6] = {A0, A1, A2, A3, A4, A5}; // 6 sensor inputs
int sensorValues[6];
int weights[6] = {-3, -2, -1, 1, 2, 3}; // Adjust weights based on sensor layout

// Base speed
int baseSpeed = 150; // Adjust as needed for your motors

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

  // Start Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Line Following Robot Initialized");
}

void loop() {
  // Step 1: Read and round off sensor values
  int activeSensors = 0;
  int positionSum = 0;
  for (int i = 0; i < 6; i++) {
    int rawValue = analogRead(sensorPins[i]);               // Read raw sensor value
    int roundedValue = ((rawValue + 50) / 100) * 100;       // Round to nearest 100
    sensorValues[i] = (roundedValue > 100) ? 1 : 0;         // Cutoff at 100
    positionSum += sensorValues[i] * weights[i];
    activeSensors += sensorValues[i];
  }

  // Step 2: Calculate position and correction
  float position = (activeSensors > 0) ? positionSum / float(activeSensors) : 0;
  float correction = position * 50; // Proportional control factor (adjust as needed)

  // Step 3: Adjust motor speeds
  int leftMotorSpeed = baseSpeed - correction;
  int rightMotorSpeed = baseSpeed + correction;

  // Constrain motor speeds
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  // Step 4: Set motor speeds
  visualizeSensors(sensorValues, 6);
  setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Control left motor
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, leftSpeed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -leftSpeed);
  }

  // Control right motor
  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, rightSpeed);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -rightSpeed);
  }
}

void visualizeSensors(int sensorValues[], int numSensors) {
  Serial.print("Sensors: ");
  for (int i = 0; i < numSensors; i++) {
    if (sensorValues[i] == 1) {
      Serial.print("|||"); // Active sensor
    } else {
      Serial.print("__"); // Inactive sensor
    }
  }
  Serial.println(); // New line for the next reading
}
