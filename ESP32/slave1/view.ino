void HomeView(char* title) {
  tft.fillScreen(BLACK);

  Text(130, 30, 3, GREEN, title);
  Text(30, 65, 4, YELLOW, "===========");

  if (strcmp(HOME_STATUS, "RUNNING") == 0) {
    NgBtn.initButton(&tft, 270, 41, 70, 40, BLACK, BLACK, RED, "NG>", 3);
    NgBtn.drawButton(false);
  }


  int x = 0;
  if (strcmp(HOME_STATUS, "MAINTENANCE") == 0) x = 70;
  else if (strcmp(HOME_STATUS, "QDC") == 0) x = 130;
  else if (strcmp(HOME_STATUS, "SETTING") == 0) x = 95;
  else if (strcmp(HOME_STATUS, "DRYING") == 0) x = 100;
  else if (strcmp(HOME_STATUS, "TRIAL") == 0) x = 110;
  else if (strcmp(HOME_STATUS, "STANDBY") == 0) x = 100;
  else if (strcmp(HOME_STATUS, "RUNNING") == 0) x = 100;

  Text(x, 115, 3, GREEN, HOME_STATUS);
  Text(50, 150, 3, WHITE, workOrderNo);

  callBtn.initButton(&tft, 160, 230, 280, 70, WHITE, RED, BLACK, "CALL", 3);
  scanBtn.initButton(&tft, 160, 320, 280, 70, WHITE, GREEN, BLACK, "SCAN", 3);

  callBtn.drawButton(false);
  scanBtn.drawButton(false);
  if (strcmp(HOME_STATUS, "STANDBY") != 0) {
    endBtn.initButton(&tft, 160, 410, 280, 70, WHITE, BLUE, BLACK, "END", 3);
    endBtn.drawButton(false);
  }

  LoadingCircle();

  CURRENT_VIEW = "HOME";
}

void CallView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  ButtonList(130, 200, 270, 340, 410, 60);
  CURRENT_VIEW = "CALL";
}

void ScanView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  CHScanBtn.initButton(&tft, 160, 140, 280, 70, WHITE, RED, BLACK, "CHANGE", 3);
  CHScanBtn.drawButton(false);

  if (manPower > 0) {
    AddScanBtn.initButton(&tft, 160, 230, 280, 70, WHITE, CYAN, BLACK, "ADD MP", 3);
    AddScanBtn.drawButton(false);
  }

  if (callCount > 0) {
    SerScanBtn.initButton(&tft, 160, 320, 280, 70, WHITE, YELLOW, BLACK, "SERVE", 3);
    SerScanBtn.drawButton(false);
  }

  // QrScanBtn.initButton(&tft, 160, 410, 280, 70, WHITE, BLUE, BLACK, "SCAN QR", 3);
  // QrScanBtn.drawButton(false);

  CURRENT_VIEW = "SCAN";
}

void ChangeView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  RunChangeBtn.initButton(&tft, 160, 130, 280, 50, WHITE, GREEN, BLACK, "RUNNING", 3);
  RunChangeBtn.drawButton(false);
  ButtonList(190, 250, 310, 370, 430, 50);

  CURRENT_VIEW = "CHANGE";
}

void ServeView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);
  ButtonList(130, 200, 270, 340, 410, 60);

  CURRENT_VIEW = "SERVE";
}

void RFIDView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  Text(60, 120, 4, YELLOW, "SILAHKAN");
  Text(60, 170, 4, YELLOW, "TAP RFID");
  Text(110, 220, 4, YELLOW, "ANDA");

  int xStatus = 0;
  if (RFID_STATUS == "MAINTENANCE") {
    xStatus = 65;
  } else if (RFID_STATUS == "MOLD") {
    xStatus = 120;
  } else if (RFID_STATUS == "TEKNISI") {
    xStatus = 95;
  } else if (RFID_STATUS == "MATERIAL") {
    xStatus = 90;
  } else if (RFID_STATUS == "QUALITY" || RFID_STATUS == "RUNNING") {
    xStatus = 100;
  } else if (RFID_STATUS == "MP") {
    xStatus = 140;
  }

  Text(xStatus, 270, 3, GREEN, RFID_STATUS);

  CURRENT_VIEW = "RFID";
}


void QrView(char* title) {
  tft.fillScreen(WHITE);

  Header(title);

  Text(45, 370, 3, BLACK, "SILAHKAN SCAN");
  // Text(65, 160, 3, BLACK, "QR BERIKUT");

  uint8_t qrcodeData[qrcode_getBufferSize(QRcode_Version)];

  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, ECC_HIGH, QR_VALUE);

  // Display QR Code on TFT
  uint8_t x0 = 115;
  uint8_t y0 = 220;

  uint8_t qr_size = 5;
  uint8_t qr_top = 100;
  uint8_t qr_left = (320 / 2) - int((qrcode.size * qr_size) / 2);  // Center

  // Each line of modules
  for (uint8_t y = 0; y < qrcode.size; y++) {
    // Each horizontal module
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {

        tft.fillRect(qr_left + (qr_size * x), qr_top + (qr_size * y), qr_size, qr_size, BLACK);
      }
    }
  }

  CURRENT_VIEW = "QR";
}

// SELECT PART FOR INPUT NG VIEW
void SelectPartView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  deserializeJson(docSlave1ForItems, listItems);
  Serial.println(listItems);

  if (!docSlave1ForItems.isNull()) {
    JsonArray jsonArray = docSlave1ForItems.as<JsonArray>();
    listItemsLength = jsonArray.size();
    Serial.println(listItemsLength);

    if (listItemsLength > 0) {
      ItemButtons =  new Adafruit_GFX_Button[listItemsLength];

      for (int i = 0; i < listItemsLength; i++) {
        JsonVariant v = jsonArray[i];
        int itemId = v["item_id"];
        const char* itemCode = v["item_code"];
        const char* itemName = v["item_name"];
        int okQuantity = v["ok_quantity"];
        int ngQuantity = v["ng_quantity"];

        Text(10, 100 + i * 95, 2, WHITE, itemName);
        ItemButtons[i].initButton(&tft, 160, 160 + i * 95, 280, 50, WHITE, WHITE, BLACK, const_cast<char*>(itemCode), 2);
        ItemButtons[i].drawButton(false);

        //CONVERT INT TO CONST CHAR
        addElementToListItemsId(String(itemId).c_str());

      }
    }
  }
}

//LIST NG ITEMS VIEW
void NGView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);
  int yPos = 120;
  int yPos2 = 120;
  for (int i = 0; i < 5; i++) {
    NgListBtn1[i].initButton(&tft, 85, yPos, 135, 65, colorButtonNgList1[i], colorButtonNgList1[i], BLACK, buttonNgList1[i], 2);
    NgListBtn1[i].drawButton(false);
    yPos = yPos + 70;
  }

  for (int i = 0; i < 5; i++) {
    NgListBtn2[i].initButton(&tft, 235, yPos2, 135, 65, colorButtonNgList2[i], colorButtonNgList2[i], BLACK, buttonNgList2[i], 2);
    NgListBtn2[i].drawButton(false);
    yPos2 = yPos2 + 70;
  }

  CURRENT_VIEW = "NG";
}