#include <SPI.h>
#include <MFRC522.h>
#include "Keyboard.h"

#define RST_PIN 5
#define SS_PIN  10
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // write_setup();
  read_setup();
}

void loop() {
  // write();
  read();
}