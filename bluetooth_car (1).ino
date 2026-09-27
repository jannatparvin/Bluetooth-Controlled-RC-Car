/*
 * Bluetooth Controlled Car - Arduino Uno
 * HC-05 Bluetooth Module + L298N Motor Driver (2 DC motors) + Servo (sensor mount)
 * Powered by Li-ion battery pack (e.g. 2x 3.7V cells in series -> ~7.4V into L298N)
 *
 * Bluetooth Commands (send from any "Bluetooth RC Car" app, or Serial Monitor):
 *   F = Forward
 *   B = Backward
 *   L = Turn Left
 *   R = Turn Right
 *   S = Stop
 *
 * HC-05 Wiring:
 *   HC-05 VCC -> 5V (or 3.3V depending on your module)
 *   HC-05 GND -> GND
 *   HC-05 TXD -> Arduino RX (pin 10, via SoftwareSerial)
 *   HC-05 RXD -> Arduino TX (pin 11, via SoftwareSerial) -- use a voltage divider,
 *                HC-05 RXD is usually 3.3V logic only
 *
 * L298N Wiring:
 *   ENA -> pin 5 (PWM speed, Left motor)
 *   IN1 -> pin 6
 *   IN2 -> pin 7
 *   ENB -> pin 3 (PWM speed, Right motor)
 *   IN3 -> pin 8
 *   IN4 -> pin 4
 *   L298N 12V/VCC -> battery positive
 *   L298N GND -> battery negative AND Arduino GND (common ground is essential)
 *
 * Servo Wiring:
 *   Signal -> pin 9
 *   VCC -> 5V
 *   GND -> GND
 */

#include <SoftwareSerial.h>
#include <Servo.h>

// Bluetooth pins
#define BT_RX 10   // connects to HC-05 TXD
#define BT_TX 11   // connects to HC-05 RXD (through voltage divider)
SoftwareSerial bluetooth(BT_RX, BT_TX);

// Left motor (Motor A on L298N)
#define ENA 5
#define IN1 6
#define IN2 7

// Right motor (Motor B on L298N)
#define ENB 3
#define IN3 8
#define IN4 4

// Servo (sensor mount)
#define SERVO_PIN 9
Servo sensorServo;

// Adjust this if your car is too fast/slow (0-255)
int motorSpeed = 200;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600); // default HC-05 baud rate

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  sensorServo.attach(SERVO_PIN);
  sensorServo.write(90); // center position

  stopCar();
  Serial.println("Bluetooth car ready. Waiting for commands...");
}

void loop() {
  if (bluetooth.available() > 0) {
    char command = bluetooth.read();
    Serial.print("Received: ");
    Serial.println(command);
    handleCommand(command);
  }
}

void handleCommand(char command) {
  switch (command) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'S':
      stopCar();
      break;
    default:
      // Ignore any unrecognized character (apps sometimes send extra bytes)
      break;
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnLeft() {
  // Left motor backward, right motor forward -> spins left
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnRight() {
  // Left motor forward, right motor backward -> spins right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
