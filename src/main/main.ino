// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"
#include "AirControl.h"
#include "EEPROM.h"

// Pinout
#define RF24_CE_PIN 9
#define RF24_CSN_PIN 10 //Pin 10 must be output to work as SPI master
#define L298N_ENA_PIN 3 //Must be PWM pin to control speed
#define L298N_IN1_PIN 4
#define L298N_IN2_PIN 2
#define L298N_IN3_PIN 8
#define L298N_IN4_PIN 7
#define HEADLIGHT_PIN A0
#define BACKLIGHT_PIN 5
#define BUZZER_PIN 6
#define RESET_PIN A7

// Time intervals for connection
#define INTERVAL_MS_SIGNAL_LOST 1000
#define INTERVAL_MS_SIGNAL_RETRY 250

// Joystick debug & security
#define DEBUG false
#define SECURITY_KEY "hibit"

struct motor {
  byte speed = 0;

  struct {
    byte input1 = LOW;
    byte input2 = LOW;
  } direction;
};

motor motorDirection, motorSpeed; // Motors

air_control airControl;  // Air Control joystick

RF24 radio(RF24_CE_PIN, RF24_CSN_PIN); // NRF24L01

// Latest signal time
unsigned long lastSignalMillis = 0;

void setup()
{
  if (DEBUG) {
    Serial.begin(115200);
  }

  // Init communication
  initNrf24();

  // Set PWM & direction pins to output for both motor
  pinMode(L298N_ENA_PIN, OUTPUT);
  pinMode(L298N_IN1_PIN, OUTPUT);
  pinMode(L298N_IN2_PIN, OUTPUT);
  pinMode(L298N_IN3_PIN, OUTPUT);
  pinMode(L298N_IN4_PIN, OUTPUT);

  // Other pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HEADLIGHT_PIN , OUTPUT);
  pinMode(BACKLIGHT_PIN , OUTPUT);

  // Init motors with default values
  sendToSpeedMotor();
  sendToDirectionMotor();
}

void loop()
{
  if (radio.available() > 0) {
    radio.read(&airControl, sizeof(air_control));

    if (strcmp(airControl.key, SECURITY_KEY) != 0) {
      invalidConnection(); // Skip if security code is not used
    } else {
      if (DEBUG) {
        debug(airControl);
      }

      control();

      lastSignalMillis = millis();
    }
  }

  if (lastSignalMillis != 0 && millis() - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) {
    lostConnection();
  }
}

void invalidConnection()
{
  lightsOn();
  turnNone();
  brakeUntilStop();
  lightsOff();
}

void lostConnection()
{
  lightsOn();
  turnNone();
  brakeUntilStop();
  initNrf24();

  delay(100);

  lightsOff();
}
