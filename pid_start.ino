// Motor control pins (L298N)
#define ENA 3    // Speed control for Left Motor (PWM)
#define IN1 5    // Direction control for Left Motor
#define IN2 6    // Direction control for Left Motor

#define ENB 11   // Speed control for Right Motor (PWM)
#define IN3 9    // Direction control for Right Motor
#define IN4 10   // Direction control for Right Motor

// Sensor pins
const int sensorPins[6] = {A0, A1, A2, A3, A4, A5}; // 6 sensor inputs
int thresholds[6] = {513, 323, 331, 357, 430, 432}; // Thresholds for each sensor
int weights[6] = {-3, -2, -1, 1, 2, 3}; // Adjust weights based on sensor layout

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
  Serial.println("Line Following Robot with PID Initialized.");
}

// PID constants
int kp = 25.0;   // Proportional gain (increase for sharper turns)
int ki = 0.0;    // Integral gain (use only if drift occurs)
int kd = 20.0;   // Derivative gain (increase to reduce oscillations)

// Motor speed settings
int baseSpeed = 70; // Base motor speed
int maxSpeed = 130;  // Maximum motor speed
int minSpeed = 50;   // Minimum motor speed for sharp turns
int leftMotorErrorCorrection = 20;
int rightMotorErrorCorrection = 0;

// PID variables
float error = 0;
int previousError = 0;
int integral = 0;

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

  // // Handle no sensors active (robot off the line)
  // if (activeSensors == 0) {
  //   // Spin in place to find the line
  //   setMotorSpeeds(-80, 80); // Turn in place
  //   delay(100); // Small delay for stability
  //   return; // Skip further processing
  // }

  // Step 2: Calculate PID error terms
  error = (activeSensors > 0) ? positionSum / float(activeSensors) : 0;
  // integral += error;                              // Accumulate error over time
  int derivative = error - previousError;      // Calculate change in error
  previousError = error;                         // Update previous error

  // Step 3: Calculate correction using PID formula
  int correction = (kp * error) + (kd * derivative); // + (ki * integral)

  // Dynamic base speed adjustment for sharp turns
  int dynamicBaseSpeed = (abs(error) > 2) ? minSpeed : baseSpeed;

  // Step 4: Determine motor speeds based on correction
  int leftMotorSpeed = dynamicBaseSpeed - correction;
  int rightMotorSpeed = dynamicBaseSpeed + correction;
  Serial.print("leftMotorSpeed: ");
  Serial.println(leftMotorSpeed);
  // Serial.print("dynamicBaseSpeed: ");
  // Serial.println(dynamicBaseSpeed);
  Serial.print("rightMotorSpeed: ");
  Serial.println(rightMotorSpeed);
  // Constrain motor speeds to valid range
  // leftMotorSpeed = constrain(leftMotorSpeed, 0, maxSpeed);
  // rightMotorSpeed = constrain(rightMotorSpeed, 0, maxSpeed);

  // Step 5: Set motor speeds
  // setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

  // Visualize sensor states
  visualizeSensors(sensorBinary, 6);

  delay(300); // Small delay for stability
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
