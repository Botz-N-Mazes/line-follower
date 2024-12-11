// Motor control pins (L298N)
#define ENA 3    // Speed control for Left Motor (PWM)
#define IN1 5    // Direction control for Left Motor
#define IN2 6    // Direction control for Left Motor

#define ENB 11   // Speed control for Right Motor (PWM)
#define IN3 9    // Direction control for Right Motor
#define IN4 10   // Direction control for Right Motor

const int leftMotorErrorCorrection = 25;
const int rightMotorErrorCorrection = 0;
int baseSpeed = 120;

void setup() {
  Serial.begin(9600);
}

void loop() {
  setMotorSpeeds(baseSpeed, baseSpeed);
  delay(1000);
  setMotorSpeeds(-baseSpeed, -baseSpeed);
  delay(1000);
}

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