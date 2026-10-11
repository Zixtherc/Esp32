#include <Arduino.h>

#define LED_PIN 2
#define SIGNALING_PIN 17
#define BUZZER_PIN 16

const unsigned long BEEP_MS = 2000;  
bool alarmOn = false;
unsigned long alarmStart = 0;
int lastState = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SIGNALING_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN,HIGH);

}

void loop() {
  int motionState = digitalRead(SIGNALING_PIN);

  if (motionState == HIGH && lastState == LOW) {
    alarmOn = true;
    alarmStart = millis();
    digitalWrite(BUZZER_PIN, LOW); 
    Serial.println("Зашёл");
  }

  if (motionState == LOW && lastState == HIGH) {
    Serial.println("Тихо");
  }

  lastState = motionState;

  if (alarmOn && millis() - alarmStart >= BEEP_MS) {
    alarmOn = false;
    digitalWrite(BUZZER_PIN, HIGH);
  }

  digitalWrite(LED_PIN, motionState);
  delay(10);
}