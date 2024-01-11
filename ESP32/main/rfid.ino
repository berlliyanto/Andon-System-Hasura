void rfidOperation(String rfid, String json) {
  if (rfid != "") {
    Serial.println(rfid);

    //CHANGE
    if (strcmp(stateFromSlave1, "change") == 0) {
      sendLoadingState("isLoading");
      delay(1000);
      if (strcmp(typeFromSlave1, "RUNNING") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "running", rfid));
        Serial.println("CHANGE RUNNING");
      } else if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "maintenance", rfid));
        Serial.println("CHANGE MAINTENANCE");
      } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "qdc", rfid));
        Serial.println("CHANGE MOLD");
      } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "setting", rfid));
        Serial.println("CHANGE TEKNISI");
      } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "drying", rfid));
        Serial.println("CHANGE MATERIAL");
      } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
        postAPI("/api/workcenterstatus", hostDev, makeBody("change", "trial", rfid));
        Serial.println("CHANGE QUALITY");
      }
    }

    //SERVE
    else if (strcmp(stateFromSlave1, "serve") == 0) {
      sendLoadingState("isLoading");
      delay(1000);
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
      sendLoadingState("isLoading");
      delay(1000);
      postAPI("/api/workcenterstatus/addmp", hostDev, makeBody("add_mp", "", rfid));
      Serial.println("add_mp");
    }
  }
}

void readRFID(String json) {
  String rfidResult = "";

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

      rfidOperation(rfidResult, json);

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}