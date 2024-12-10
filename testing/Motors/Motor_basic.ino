// Define motor control pins
#define ENA 6  // Speed control for Motor 1
#define IN1 2  // Direction control for Motor 1
#define IN2 4  // Direction control for Motor 1

#define ENB 3  // Speed control for Motor 2
#define IN3 7  // Direction control for Motor 2
#define IN4 8  // Direction control for Motor 2

void setup() {
  // Set motor control pins as outputs
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start with motors stopped
  stopMotors();
}

void loop() {
  // Run both motors from speed 80 to max speed (255)
  for (int speed = 80; speed <= 255; speed += 20) { // Increment speed by 20
    Serial.print("Running motors at speed: ");
    Serial.println(speed);

    // Motor 1 - Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed); // Set speed for Motor 1

    // Motor 2 - Forward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed); // Set speed for Motor 2

    delay(2000); // Run at current speed for 2 seconds
  }

  // Stop both motors
  stopMotors();
  Serial.println("Motors stopped.");
  delay(5000); // Wait for 5 seconds before restarting
}

void stopMotors() {
  // Stop Motor 1
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Stop Motor 2
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}
