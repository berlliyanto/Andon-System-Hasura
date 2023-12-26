void rfidOperation(String rfid) {
  if (rfid != "") {
    Serial.println(rfid);
    if (strcmp(stateFromSlave1, "change") == 0) {
      Serial.println("change");
      if (strcmp(typeFromSlave1, "RUNNING") == 0) {
        Serial.println("RUNNING");
      } else if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
        Serial.println("MAINTENANCE");
      } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
        Serial.println("MOLD");
      } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
        Serial.println("TEKNISI");
      } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
        Serial.println("MATERIAL");
      } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
        Serial.println("QUALITY");
      }
    } else if (strcmp(stateFromSlave1, "serve") == 0) {
      Serial.println("serve");
      if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
        Serial.println("MAINTENANCE");
      } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
        Serial.println("MOLD");
      } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
        Serial.println("TEKNISI");
      } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
        Serial.println("MATERIAL");
      } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
        Serial.println("QUALITY");
      }
    } else if (strcmp(stateFromSlave1, "add_mp") == 0) {
      Serial.println("add_mp");
    } else if (strcmp(stateFromSlave1, "running") == 0) {
      Serial.println("running");
    } else if (strcmp(stateFromSlave1, "end") == 0) {
      Serial.println("End");
    } else if (strcmp(stateFromSlave1, "standby") == 0) {
      Serial.println("Standby");
    }
  }
}

void readRFID() {
  String rfidResult = "";  // Variabel untuk menampung hasil pembacaan RFID

  if (rfid.PICC_IsNewCardPresent()) {  // new tag is available
    if (rfid.PICC_ReadCardSerial()) {  // NUID has been read
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // Build UID in the formatted hex format
      for (int i = 0; i < rfid.uid.size; i++) {
        rfidResult += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");  // add leading zero if necessary
        rfidResult += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) {
          rfidResult += "-";
        }
      }

      rfidResult.toUpperCase();

      rfidOperation(rfidResult);

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}