//OPERATION AFTER RFID HAS A VALUE
void rfidOperation(String rfid, String json) {
  if (rfid != "") {
    isListenStatus = true;
    Serial.println(rfid);

    //CHANGE
    if (strcmp(stateFromSlave1, "change") == 0) {
      if (strcmp(typeFromSlave1, "RUNNING") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "running", rfid));
        // String json = "{\"state\":\"change\",\"type\":\"RFID\",\"status\":\"running\",\"value\":\"" + rfid + "\",\"work_center_id\":\"" + String(workCenterId) + "\"}";
        // writeToSerial1(json);
        Serial.println("CHANGE RUNNING");
      } else if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "maintenance", rfid ));
        Serial.println("CHANGE MAINTENANCE");
      } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
        String json = "{\"state\":\"change\",\"type\":\"RFID\",\"status\":\"qdc\",\"value\":\"" + rfid + "\",\"work_center_id\":\"" + String(workCenterId) + "\"}";
        writeToSerial1(json);
        // postAPI("/api/workcenterstatus", hostDev, makeBody("change", "qdc", rfid));
        Serial.println("CHANGE MOLD");
      } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
        String json = "{\"state\":\"change\",\"type\":\"RFID\",\"status\":\"setting\",\"value\":\"" + rfid + "\",\"work_center_id\":\"" + String(workCenterId) + "\"}";
        writeToSerial1(json);
        // postAPI("/api/workcenterstatus", hostDev, makeBody("change", "setting", rfid));
        Serial.println("CHANGE TEKNISI");
      } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
        String json = "{\"state\":\"change\",\"type\":\"RFID\",\"status\":\"drying\",\"value\":\"" + rfid + "\",\"work_center_id\":\"" + String(workCenterId) + "\"}";
        writeToSerial1(json);
        // postAPI("/api/workcenterstatus", hostDev, makeBody("change", "drying", rfid));
        Serial.println("CHANGE MATERIAL");
      } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
        String json = "{\"state\":\"change\",\"type\":\"RFID\",\"status\":\"trial\",\"value\":\"" + rfid + "\",\"work_center_id\":\"" + String(workCenterId) + "\"}";
        writeToSerial1(json);
        // postAPI("/api/workcenterstatus", hostDev, makeBody("change", "trial", rfid));
        Serial.println("CHANGE QUALITY");
      }
    }

    //SERVE
    else if (strcmp(stateFromSlave1, "serve") == 0) {
      if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
        postAPI("/api/workcentercall", hostDev, makeBody("serve", "maintenance", rfid));
        Serial.println("SERVE MAINTENANCE");
      } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
        postAPI("/api/workcentercall", hostDev, makeBody("serve", "mold", rfid));
        Serial.println("SERVE MOLD");
      } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
        postAPI("/api/workcentercall", hostDev, makeBody("serve", "technician", rfid));
        Serial.println("SERVE TEKNISI");
      } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
        postAPI("/api/workcentercall", hostDev, makeBody("serve", "material", rfid));
        Serial.println("SERVE MATERIAL");
      } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
        postAPI("/api/workcentercall", hostDev, makeBody("serve", "quality", rfid));
        Serial.println("SERVE QUALITY");
      }
    }

    //ADD MP
    else if (strcmp(stateFromSlave1, "add_mp") == 0) {
      postAPI("/api/workcenterstatus/addmp", hostDev, makeBody("add_mp", "", rfid));
      Serial.println("add_mp");
    }
  }
}

void readRFID(String json) {
  MFRC522::MIFARE_Key key;
  for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
    // Copy the known key into the MIFARE_Key structure
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
      key.keyByte[i] = knownKeys[k][i];
    }
    // Try the key
    if (tryKey(&key, json)) {
      break;
    }

    // http://arduino.stackexchange.com/a/14316
    if (!rfid.PICC_IsNewCardPresent())
      break;
    if (!rfid.PICC_ReadCardSerial())
      break;
  }

  // THIS PROGRAM GIVE 4 BLOCK RESULT
  // if (rfid.PICC_IsNewCardPresent()) {  // new tag is available
  //   if (rfid.PICC_ReadCardSerial()) {  // NUID has been read
  //     MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //     Serial.print("RFID/NFC Tag Type: ");
  //     Serial.println(rfid.PICC_GetTypeName(piccType));

  //     Build UID in the formatted hex format
  //     for (int i = 0; i < rfid.uid.size; i++) {
  //       rfidResult += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");  // add leading zero if necessary
  //       rfidResult += String(rfid.uid.uidByte[i], HEX);
  //       if (i < rfid.uid.size - 1) {
  //         rfidResult += "-";
  //       }
  //     }

  //     rfidResult.toUpperCase();

  //     rfidOperation(rfidResult, json);

  //     rfid.PICC_HaltA();
  //     rfid.PCD_StopCrypto1();
  //   }
  // }
}

bool tryKey(MFRC522::MIFARE_Key *key, String json) {
  String rfidResult = "";
  bool result = false;
  byte buffer[18];
  byte block = 0;
  MFRC522::StatusCode status;

  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
    return false;
  }

  // Read block
  byte byteCount = sizeof(buffer);
  status = rfid.MIFARE_Read(block, buffer, &byteCount);
  if (status == MFRC522::STATUS_OK) {
    // Successful read
    result = true;
    dumpByteArray((*key).keyByte, MFRC522::MF_KEY_SIZE);
    rfidResult = dumpByteArray(buffer, 5);
    rfidResult.toUpperCase();
    rfidValue = rfidResult;
    rfidOperation(rfidResult, json);  //DEFAULTNYA INI 16, DIUBAH JADI 5 BIAR SAMA KYK RASPI
  }

  rfid.PICC_HaltA();       // Halt PICC
  rfid.PCD_StopCrypto1();  // Stop encryption on PCD
  return result;
}

String dumpByteArray(byte *buffer, byte bufferSize) {
  String result = "";
  for (byte i = 0; i < bufferSize; i++) {
    if (i > 0) {
      result += "-";
    }
    result += (buffer[i] < 0x10 ? "0" : "") + String(buffer[i], HEX);
  }
  return result;
}