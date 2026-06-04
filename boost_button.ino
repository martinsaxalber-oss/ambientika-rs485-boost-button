#include <SoftwareSerial.h>

// Pin-Definitionen
const int rxPin    = 10;   // MAX485 RO
const int txPin    = 11;   // MAX485 DI
const int deRePin  = 3;    // MAX485 DE & RE (Sende-/Empfangsumschaltung)
const int relayPin = 2;    // Eingang Koppelrelais (potenzialfreier Kontakt, NO)

SoftwareSerial fanSerial(rxPin, txPin);

// Befehlsrahmen gemaess Suedwind-Kommunikationsprotokoll (9600 8N1)
byte autoCmd[]  = {0x02,0x30,0x31,0x36,0x36,0x30,0x30,0x36,0x37,0x03}; // Automatik Tag, Abluft Master
byte pauseCmd[] = {0x02,0x30,0x31,0x32,0x30,0x30,0x30,0x32,0x31,0x03}; // Motor Pause, Klappe geoeffnet
byte boostCmd[] = {0x02,0x30,0x31,0x32,0x37,0x30,0x30,0x32,0x36,0x03}; // Abluft Master Stufe 3 (Boost)

enum State { NORMAL, PAUSING_TO_BOOST, BOOSTING, PAUSING_TO_NORMAL };
State currentState = NORMAL;

unsigned long stateStartTime = 0;
unsigned long lastSendTime   = 0;
const unsigned long interval       = 500;              // 500 ms Sendeintervall (Pflicht)
const unsigned long pauseDuration  = 10000;            // 10 s Motorpause vor Richtungs-/Stufenwechsel
const unsigned long boostDuration  = 20UL * 60 * 1000; // 20 Minuten Boost

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;                // 50 ms Entprellzeit

void setup() {
  pinMode(relayPin, INPUT_PULLUP);
  pinMode(deRePin, OUTPUT);
  digitalWrite(deRePin, LOW);  // MAX485 im Empfangsmodus starten
  fanSerial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Taster mit Flankenerkennung und Entprellung
  int reading = digitalRead(relayPin);
  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {              // Taster gedrueckt
        if (currentState == NORMAL) {
          currentState = PAUSING_TO_BOOST;   // 1. Druck: Boost starten
          stateStartTime = currentMillis;
        } else if (currentState == BOOSTING) {
          currentState = PAUSING_TO_NORMAL;  // 2. Druck: Boost abbrechen
          stateStartTime = currentMillis;
        }
      }
    }
  }
  lastButtonState = reading;

  // 2. Zeitgesteuerte Zustandswechsel (millis-Ueberlauf-sicher)
  if (currentState == PAUSING_TO_BOOST) {
    if (currentMillis - stateStartTime >= pauseDuration) {
      currentState = BOOSTING;
      stateStartTime = currentMillis;
    }
  } else if (currentState == PAUSING_TO_NORMAL) {
    if (currentMillis - stateStartTime >= pauseDuration) {
      currentState = NORMAL;
    }
  } else if (currentState == BOOSTING) {
    if (currentMillis - stateStartTime >= boostDuration) {
      currentState = PAUSING_TO_NORMAL;      // automatisches Ende nach 20 Minuten
      stateStartTime = currentMillis;
    }
  }

  // 3. Aktiven Befehl exakt alle 500 ms senden
  if (currentMillis - lastSendTime >= interval) {
    lastSendTime = currentMillis;
    digitalWrite(deRePin, HIGH);             // MAX485 auf Senden
    if (currentState == NORMAL) {
      fanSerial.write(autoCmd, sizeof(autoCmd));
    } else if (currentState == PAUSING_TO_BOOST || currentState == PAUSING_TO_NORMAL) {
      fanSerial.write(pauseCmd, sizeof(pauseCmd));
    } else if (currentState == BOOSTING) {
      fanSerial.write(boostCmd, sizeof(boostCmd));
    }
    delay(2);                                // Leitung freilaufen lassen
    digitalWrite(deRePin, LOW);              // zurueck auf Empfangen
  }
}
