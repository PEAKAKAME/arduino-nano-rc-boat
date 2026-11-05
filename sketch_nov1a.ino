

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11);
Servo rudder;

#define ENA 5
#define IN1 6
#define IN2 7
#define SERVO_PIN 9

int motorSpeed = 60;
int steeringAngle = 90;
int direction = 0;

String command = "";

void stopMotor() {
  direction = 0;
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void runMotor(int dir) {
  direction = dir;

  if (dir == 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  analogWrite(ENA, map(motorSpeed, 0, 100, 0, 255));
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "FORWARD") {
    runMotor(1);
  }
  else if (cmd == "BACKWARD") {
    runMotor(-1);
  }
  else if (cmd == "STOP") {
    stopMotor();
  }
  else if (cmd == "LEFT") {
    steeringAngle = 45;
    rudder.write(steeringAngle);
  }
  else if (cmd == "RIGHT") {
    steeringAngle = 135;
    rudder.write(steeringAngle);
  }
  else if (cmd == "CENTER") {
    steeringAngle = 90;
    rudder.write(steeringAngle);
  }
  else if (cmd.startsWith("SPEED ")) {
    int value = cmd.substring(6).toInt();

    if (value >= 0 && value <= 100) {
      motorSpeed = value;

      if (direction != 0) {
        runMotor(direction);
      }
    }
  }
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  bluetooth.begin(9600);

  rudder.attach(SERVO_PIN);
  rudder.write(90);

  stopMotor();
}

void loop() {
  while (bluetooth.available()) {
    char c = bluetooth.read();

    if (c == '\n') {
      processCommand(command);
      command = "";
    }
    else if (c != '\r') {
      if (command.length() < 40) {
        command += c;
      } else {
        command = "";
      }
    }
  }
}

