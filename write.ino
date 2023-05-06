void write_setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  while (!Serial) {}
  Serial.println(F("(фигня для записи пароля) "));
}

void write() {
  Serial.println();

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte buffer[16] = {};
  Serial.println(F("Введите пароль: "));
  while (1) {
    if (Serial.available() > 0) {
      Serial.readBytesUntil(13, (char *) buffer, 16);
      break;
    }
  }

  Serial.println(F("Ожидание карты... "));
  while (1) {
    if (!mfrc522.PICC_IsNewCardPresent()) { continue; }
    if ( !mfrc522.PICC_ReadCardSerial() ) { continue; }
    break;
  } Serial.println();
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(mfrc522.PICC_GetTypeName(mfrc522.PICC_GetType(mfrc522.uid.sak)));

  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Произошла ошибка авторизации: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else Serial.println(F("Авторизация успешна"));

  status = mfrc522.MIFARE_Write(1, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Произошла ошибка записи: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("Запись успешна"));
}