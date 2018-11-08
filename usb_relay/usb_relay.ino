#include <EEPROM.h>
#include "FiniteStateMachine.h"

#define RELAY_PIN 11
#define LED_PIN 13
#define EEPROM_TIMEOUT_ADDRESS 0

void onStart();
void onUpdate();

void offStart();
void offUpdate();


State onState = State(onStart, onUpdate, NULL);
State offState = State(offStart, offUpdate, NULL);


FSM stateMachine = FSM(offState);

int timeout = 10;

long onStartTime = 0;

int incomingByte = 0;

void setup() {

  Serial.begin(9600);

  delay(1000);

  timeout = EEPROM.read(EEPROM_TIMEOUT_ADDRESS);

  Serial.println("Start");
  Serial.print("Timeout:");
  Serial.println(timeout);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  stateMachine.update();

  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    if (incomingByte == 'A') {
      stateMachine.transitionTo(onState);
    } else if (incomingByte == 'Z') {
      stateMachine.transitionTo(offState);
    } else if (incomingByte == 'O') {

      if (Serial.available() > 0) {
        incomingByte = Serial.read();

        EEPROM.write(EEPROM_TIMEOUT_ADDRESS, incomingByte);
        Serial.print("Setup timeout:");
        Serial.print(incomingByte);
        Serial.println(".");

      }
    }

  }

}


void onStart() {
  Serial.println("Relay on.");
  onStartTime = millis();
}

void onUpdate() {
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);


  if (millis() - onStartTime > 1000 * timeout) {
    stateMachine.transitionTo(offState);
  }
}

void offStart() {
  Serial.println("Relay off.");

}

void offUpdate() {
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}
