
#include <Servo.h>
#include <SoftwareSerial.h>

// Bluetooth RX, TX
SoftwareSerial bluetooth(10, 11);

// Servo
Servo rudder;

#define SERVO_PIN 9

int steeringAngle = 90;

String command = "";

void processCommand(String cmd) {

  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "LEFT") {

    steeringAngle = 60;
    rudder.write(steeringAngle);

    bluetooth.println("SERVO MOVED LEFT");
  }

  else if (cmd == "RIGHT") {

    steeringAngle = 170;
    rudder.write(steeringAngle);

    bluetooth.println("SERVO MOVED RIGHT");
  }

  else if (cmd == "CENTER") {

    steeringAngle = 90;
    rudder.write(steeringAngle);

    bluetooth.println("SERVO CENTERED");
  }

  else if (cmd.startsWith("ANGLE ")) {

    String angleText = cmd.substring(6);
    angleText.trim();

    bool valid = angleText.length() > 0;

    for (unsigned int i = 0; i < angleText.length(); i++) {
      if (!isDigit(angleText[i])) {
        valid = false;
        break;
      }
    }

    if (valid) {
      int angle = angleText.toInt();

      if (angle >= 0 && angle <= 180) {

        steeringAngle = angle;
        rudder.write(steeringAngle);

        bluetooth.print("SERVO ANGLE: ");
        bluetooth.println(steeringAngle);
      }
      else {
        bluetooth.println("INVALID ANGLE: USE 0-180");
      }
    }
    else {
      bluetooth.println("INVALID ANGLE");
    }
  }

  else if (cmd == "PING") {

    bluetooth.println("ARDUINO ONLINE");
  }

  else {

    bluetooth.println("UNKNOWN COMMAND");
  }
}

void setup() {

  bluetooth.begin(9600);

  rudder.attach(SERVO_PIN);

  rudder.write(90);

  bluetooth.println("==================");
  bluetooth.println("SERVO SYSTEM READY");
  bluetooth.println("BLUETOOTH CONNECTED");
  bluetooth.println("SERVO CENTERED");
  bluetooth.println("==================");
}

void loop() {

  while (bluetooth.available()) {

    char c = bluetooth.read();

    if (c == '\n' || c == '\r') {

      if (command.length() > 0) {

        processCommand(command);

        command = "";
      }
    }

    else if (command.length() < 40) {

      command += c;
    }

    else {

      command = "";

      bluetooth.println("COMMAND TOO LONG");
    }
  }
}
