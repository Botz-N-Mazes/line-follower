// PID Constants
float Kp = 1.0;  // Proportional gain
float Ki = 0.1;  // Integral gain
float Kd = 0.05; // Derivative gain

// Variables for PID
float previousError = 0;
float integral = 0;

// Motor speeds
int baseSpeed = 150; // Adjust base speed as needed
int leftMotorSpeed, rightMotorSpeed;

// Sensor pins
const int sensorPins[5] = {A0, A1, A2, A3, A4}; // Replace with actual pins
int sensorValues[5];
int weights[5] = {-2, -1, 0, 1, 2};

// Motor control pins
const int leftMotorForward = 5;
const int leftMotorBackward = 6;
const int rightMotorForward = 9;
const int rightMotorBackward = 10;

void setup() {
  // Initialize motor pins as outputs
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);

  // Initialize sensor pins as inputs
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  Serial.begin(9600); // For debugging
}

void loop() {
  // Step 1: Read sensor values
  int activeSensors = 0;
  int positionSum = 0;
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(sensorPins[i]) > 500 ? 1 : 0; // Threshold of 500
    positionSum += sensorValues[i] * weights[i];
    activeSensors += sensorValues[i];
  }

  // Step 2: Calculate position and error
  float position = (activeSensors > 0) ? positionSum / float(activeSensors) : previousError;
  float error = position - 0; // Target position is 0 (center)

  // Step 3: PID calculations
  float proportional = error;
  integral += error;
  float derivative = error - previousError;
  float correction = (Kp * proportional) + (Ki * integral) + (Kd * derivative);

  // Step 4: Adjust motor speeds
  leftMotorSpeed = baseSpeed + correction;
  rightMotorSpeed = baseSpeed - correction;

  // Constrain motor speeds
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  // Step 5: Set motor speeds
  setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

  // Update previous error
  previousError = error;

  // Debugging output
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" Correction: ");
  Serial.println(correction);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Control left motor
  if (leftSpeed > 0) {
    analogWrite(leftMotorForward, leftSpeed);
    analogWrite(leftMotorBackward, 0);
  } else {
    analogWrite(leftMotorForward, 0);
    analogWrite(leftMotorBackward, -leftSpeed);
  }

  // Control right motor
  if (rightSpeed > 0) {
    analogWrite(rightMotorForward, rightSpeed);
    analogWrite(rightMotorBackward, 0);
  } else {
    analogWrite(rightMotorForward, 0);
    analogWrite(rightMotorBackward, -rightSpeed);
  }
}
