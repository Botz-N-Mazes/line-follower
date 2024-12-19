// Motor control pins (L298N)
#define ENA 3    // Speed control for Left Motor (PWM)
#define IN1 5    // Direction control for Left Motor
#define IN2 6    // Direction control for Left Motor

#define ENB 11   // Speed control for Right Motor (PWM)
#define IN3 9    // Direction control for Right Motor
#define IN4 10   // Direction control for Right Motor

#include <SoftwareSerial.h>

// Bluetooth module pins
SoftwareSerial BTSerial(2, 8); // RX (pin 2), TX (pin 8)

int leftMotorErrorCorrection = 0;  // Left motor error correction
int rightMotorErrorCorrection = 0; // Right motor error correction
int baseSpeed = 50;                // Default base speed
int delayDuration = 3000;          // Default delay between motor actions

void setup() {
  // Initialize motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  BTSerial.begin(9600); // Initialize Bluetooth communication
  Serial.begin(9600);   // For debugging
  Serial.println("Line Following Robot Ready. Send commands via Bluetooth.");
  BTSerial.println("Robot Ready. Send commands to adjust settings.");
}

void loop() {
  // Check for and handle Bluetooth commands
  handleBluetoothCommands();
  setMotorSpeeds(baseSpeed, baseSpeed);

  // Perform motor actions with current settings
  // for (int i = 0; i < 6; i++) {
  //   int MotorSpeed = baseSpeed + i * 15;
  //   MotorSpeed = constrain(MotorSpeed, 0, 255);
  //   setMotorSpeeds(MotorSpeed, -MotorSpeed);
  //   delay(delayDuration);
  // }
}

// Function to handle Bluetooth commands
void handleBluetoothCommands() {
  if (BTSerial.available() > 0) {
    String command = BTSerial.readStringUntil('\n'); // Read command
    command.trim(); // Remove whitespace or newline characters

    // Handle speed change (Sxxx)
    if (command.startsWith("S")) {
      baseSpeed = command.substring(1).toInt();
      baseSpeed = constrain(baseSpeed, 0, 255); // Ensure valid range
      BTSerial.print("Base speed set to: ");
      BTSerial.println(baseSpeed);
      Serial.print("Base speed set to: ");
      Serial.println(baseSpeed);

    // Handle delay change (Dxxx)
    } else if (command.startsWith("D")) {
      delayDuration = command.substring(1).toInt();
      delayDuration = max(delayDuration, 0); // Ensure non-negative delay
      BTSerial.print("Delay set to: ");
      BTSerial.println(delayDuration);
      Serial.print("Delay set to: ");
      Serial.println(delayDuration);

    // Handle left motor error correction (LSxxx)
    } else if (command.startsWith("LS")) {
      leftMotorErrorCorrection = command.substring(2).toInt();
      BTSerial.print("Left motor error correction set to: ");
      BTSerial.println(leftMotorErrorCorrection);
      Serial.print("Left motor error correction set to: ");
      Serial.println(leftMotorErrorCorrection);

    // Handle right motor error correction (RSxxx)
    } else if (command.startsWith("RS")) {
      rightMotorErrorCorrection = command.substring(2).toInt();
      BTSerial.print("Right motor error correction set to: ");
      BTSerial.println(rightMotorErrorCorrection);
      Serial.print("Right motor error correction set to: ");
      Serial.println(rightMotorErrorCorrection);

    // Handle invalid commands
    } else {
      BTSerial.println("ERR: Invalid command.");
      Serial.println("ERR: Invalid command.");
    }
  }
}

// Function to set motor speeds
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Control left motor
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, constrain(leftSpeed + leftMotorErrorCorrection, 0, 255));
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, constrain(-leftSpeed + leftMotorErrorCorrection, 0, 255));
  }

  // Control right motor
  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, constrain(rightSpeed + rightMotorErrorCorrection, 0, 255));
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, constrain(-rightSpeed + rightMotorErrorCorrection, 0, 255));
  }
}
