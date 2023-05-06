void read_setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  Keyboard.begin();
  Serial.println(F("(фигня для чтения пароля) "));
}

void read() {
  Serial.println();

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  byte buffer[128] = {0};
  len = 32;

  Serial.println(F("Ожидание карты... "));
  while (1) {
    if (!mfrc522.PICC_IsNewCardPresent()) { continue; }
    if ( !mfrc522.PICC_ReadCardSerial() ) { continue; }
    break;
  } Serial.println();

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(mfrc522.PICC_GetTypeName(mfrc522.PICC_GetType(mfrc522.uid.sak)));

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Произошла ошибка авторизации: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else Serial.println(F("Авторизация успешна"));

  status = mfrc522.MIFARE_Read(1, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Ошибка чтения: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else Serial.print((char *) buffer);
  Keyboard.print((char *) buffer);

  digitalWrite(LED_BUILTIN, 0);
  delay(5000);
  digitalWrite(LED_BUILTIN, 1);
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}