String readRFID() {
  String rfidResult = ""; // Variabel untuk menampung hasil pembacaan RFID

  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been read
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // Build UID in the formatted hex format
      for (int i = 0; i < rfid.uid.size; i++) {
        rfidResult += String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""); // add leading zero if necessary
        rfidResult += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) {
          rfidResult += "-";
        }
      }

      rfidResult.toUpperCase();

      Serial.print("UID: ");
      Serial.println(rfidResult);

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      return rfidResult;
    }
  }else{
    return "";
  }
}