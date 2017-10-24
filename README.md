# Arduino-Bluetooth-Remote-Control
Arduino bluetooth control

```
Example codes are in file arduinobluetooth.ino
```

## Google Play Store Android App

https://play.google.com/store/apps/details?id=com.hamzaburakhan.btremotecontrolarduino

### Data structure

```ruby
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

```
