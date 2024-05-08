#include <SoftwareSerial.h>

#define RX_PIN D1 // Pin connected to TX of ESP32
#define TX_PIN D2 // Pin connected to RX of ESP32

#define pirSensor A0
#define magneticSensor D5
#define buzzer D6

int pirState, magneticState;
int previousState = -1; // initialize to an invalid state
unsigned long myTimes;
bool trigSensor = 0;

SoftwareSerial espSerial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);
  delay(100);

  pinMode(pirSensor, INPUT);
  pinMode(magneticSensor, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, 0);
  // debug();
  // put your setup code here, to run once:

}

void loop() {
  if (espSerial.available() > 0) {
    String response = espSerial.readStringUntil('\n'); // read the response until newline character
    Serial.println(response);
    if (response == "sensor_on") {
      // do something when "sensor_on" command is received
      Serial.println("Trig sensor 0n");
      trigSensor = 1;
    }else if (response == "sensor_off") {
      // do something when "sensor_on" command is received
      Serial.println("Trig sensor 0ff");
      trigSensor = 0;
    }
  }

  if (trigSensor) {
    pirState = analogRead(pirSensor);
    magneticState = digitalRead(magneticSensor);

    if (magneticState != previousState) {
      if (magneticState == 1) {
        Serial.println("Pintu Terbuka");
        espSerial.write("terbuka");
        digitalWrite(buzzer, 1);
        delay(2000);
        digitalWrite(buzzer, 0);
        delay(2000);
      } else if (magneticState == 0) {
        Serial.println("Pintu Tertutup");
        espSerial.write("tertutup");
        digitalWrite(buzzer, 1);
        delay(2000);
        digitalWrite(buzzer, 0);
        delay(2000);
      }
      previousState = magneticState;
    }

    if (pirState > 500) {
      Serial.println("PIR Sensor Terdeteksi !");
      espSerial.write("pir");
      while (pirState > 500) {
        pirState = analogRead(pirSensor);
        digitalWrite(buzzer, 1);
        delay(200);
      }
      delay(3000);
    } else {
      digitalWrite(buzzer, 0);
    }
  }
  delay(100);
}

void debug() {
  while (true) {
    if (millis() - myTimes >= 5000UL) {
      digitalWrite(buzzer, 1);
      delay(500);
      digitalWrite(buzzer, 0);
      myTimes = millis();
    }
    pirState = analogRead(pirSensor);
    magneticState = digitalRead(magneticSensor);
    Serial.print("PIR Sensor : ");
    Serial.println(pirState);
    Serial.print("Magnetic Sensor : ");
    Serial.println(magneticState);
    delay(100);
  }
  // put your main code here, to run repeatedly:

}
