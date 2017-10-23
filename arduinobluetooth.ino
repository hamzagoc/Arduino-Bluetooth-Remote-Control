#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
String isim = "Arduino UNO";
int sifre = 1234;
String uart = "9600,0,0";

// Gyro And Acclerometer sensor 
struct SensorValue {
    float x;
    float y;
    float z;
};

struct JoyStickValue {
    int x;
    int y;
    float angle;
    float distance;
    String eightdirection;
    String fourdirection;
};

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);

}

void loop() {

    String message = getBluetoothMessage();
    if (message != "") {
        if(message.length()>12) {
            if(message.startsWith("X")) { // gyro and acclerometer sensor values
                SensorValue vals = getSensorXYZValue(message);
                String output = "X:  ";
                output +=vals.x;
                output +="   Y: ";
                output +=vals.y;
                output +="   Z: ";
                output += vals.z;
                Serial.println(output);
            } else if(message.startsWith("R")) { // right joystick
                JoyStickValue joystickRight = getJoyStickValue(message);
                String output = "RX:  ";
                output += joystickRight.x;
                output+="  RY:  ";
                output+=joystickRight.y;
                output+="  Ang:  ";
                output+=joystickRight.angle;
                output+="  Dis:  ";
                output+=joystickRight.distance;
                output+="  4Dir:  ";
                output+=joystickRight.fourdirection;
                output+="  8Dir:  ";
                output+=joystickRight.eightdirection;
                Serial.println(output);


            } else if(message.startsWith("L")) { // left joystick
                JoyStickValue joystickLeft = getJoyStickValue(message);
                String output = "LX:  ";
                output += joystickLeft.x;
                output+="  LY:  ";
                output+=joystickLeft.y;
                output+="  Ang:  ";
                output+=joystickLeft.angle;
                output+="  Dis:  ";
                output+=joystickLeft.distance;
                output+="  4Dir:  ";
                output+=joystickLeft.fourdirection;
                output+="  8Dir:  ";
                output+=joystickLeft.eightdirection;
                Serial.println(output);

            }
        } else {
            Serial.println(message);
        }

    }

    delay(100);
}

String getBluetoothMessage() {
    String input = "";
    if (!mySerial.available()) {
        return "";
    }
    while (mySerial.available()) {
        input += (char) mySerial.read();
        if (!mySerial.available()) delayMicroseconds(50); //  Delay for messages longer than 1 character
    }
   // Serial.println(input);
    int rear = input.lastIndexOf("#");
    if (rear < 0) {
        return "";
    }

    input = input.substring(0, rear);
    int front = input.lastIndexOf("*");
    if (front < 0) {
        return "";
    } else {
        input = input.substring(front + 1);
    }
    return input;
}

SensorValue getSensorXYZValue(String sensorVal) {
    SensorValue sensorval;
    int index_x = sensorVal.indexOf("X:");
    int index_y = sensorVal.indexOf("Y:");
    int index_z = sensorVal.indexOf("Z:");
    if (index_x < 0 || index_y < 0 || index_z < 0) { // missing data
        sensorval.x = -100;
        sensorval.y = -100;
        sensorval.z = -100;
    } else {
        sensorval.x = sensorVal.substring(index_x + 2, index_y).toFloat();
        sensorval.y = sensorVal.substring(index_y + 2, index_z).toFloat();
        sensorval.z = sensorVal.substring(index_z + 2).toFloat();
    }
    //Serial.println(sensorVal);
    return sensorval;
}

JoyStickValue getJoyStickValue(String bluetoothMessage) {
    int index_x = bluetoothMessage.indexOf("X:");
    int index_y = bluetoothMessage.indexOf("Y:");
    int index_a = bluetoothMessage.indexOf("A:");
    int index_d = bluetoothMessage.indexOf("D:");
    JoyStickValue joyStick;
   // Serial.println(bluetoothMessage);
    if (index_x < 0 || index_y < 0 || index_a < 0) { // missing data
        joyStick.x = -1000;
        joyStick.y = -1000;
        joyStick.angle = -1000;
        joyStick.distance = -1000;
    } else {
        joyStick.x = bluetoothMessage.substring(index_x + 2, index_y).toInt();
        joyStick.y = bluetoothMessage.substring(index_y + 2, index_a).toInt();
        joyStick.angle = bluetoothMessage.substring(index_a + 2, index_d).toFloat();
        joyStick.distance = bluetoothMessage.substring(index_d + 2).toFloat();
        joyStick.eightdirection = get8Direction(joyStick);
        joyStick.fourdirection = get4Direction(joyStick);
    }
    return joyStick;
}

String get8Direction(JoyStickValue joystick) {
    float angle = joystick.angle;
    if (joystick.distance > 10) {
        if (angle >= 247.5 && angle < 292.5) {
            return "STICK_UP";
        } else if (angle >= 292.5 && angle < 337.5) {
            return "STICK_UPRIGHT";
        } else if (angle >= 337.5 || angle < 22.5) {
            return "STICK_RIGHT";
        } else if (angle >= 22.5 && angle < 67.5) {
            return "STICK_DOWNRIGHT";
        } else if (angle >= 67.5 && angle < 112.5) {
            return "STICK_DOWN";
        } else if (angle >= 112.5 && angle < 157.5) {
            return "STICK_DOWNLEFT  ";
        } else if (angle >= 157.5 && angle < 202.5) {
            return "STICK_LEFT";
        } else if (angle >= 202.5 && angle < 247.5) {
            return "STICK_UPLEFT";
        }
    } else if (joystick.distance <= 10) {
        return "STICK_NONE";
    }
    return "";
}

String get4Direction(JoyStickValue joystick) {
    float angle = joystick.angle;
    if (joystick.distance > 10) {
        if (angle >= 225 && angle < 315) {
            return "STICK_UP";
        } else if (angle >= 315 || angle < 45) {
            return "STICK_RIGHT";
        } else if (angle >= 45 && angle < 135) {
            return "STICK_DOWN";
        } else if (angle >= 135 && angle < 225) {
            return "STICK_LEFT";
        }
    } else if (joystick.distance <= 10) {
        return "STICK_NONE";
    }
    return "";
}
