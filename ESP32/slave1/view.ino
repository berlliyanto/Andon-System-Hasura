void HomeView(char* title) {
  tft.fillScreen(BLACK);

  Text(130, 30, 3, GREEN, title);
  Text(30, 65, 4, YELLOW, "===========");

  int x = 0;
  if (strcmp(HOME_STATUS, "MAINTENANCE") == 0) x = 70;
  else if (strcmp(HOME_STATUS, "QDC") == 0) x = 130;
  else if (strcmp(HOME_STATUS, "SETTING") == 0) x = 95;
  else if (strcmp(HOME_STATUS, "DRYING") == 0) x = 100;
  else if (strcmp(HOME_STATUS, "TRIAL") == 0) x = 110;
  else if (strcmp(HOME_STATUS, "STANDBY") == 0) x = 100;
  else if (strcmp(HOME_STATUS, "RUNNING") == 0) x = 100;

  Text(x, 115, 3, GREEN, HOME_STATUS);

  callBtn.initButton(&tft, 160, 200, 280, 70, WHITE, RED, BLACK, "CALL", 3);
  scanBtn.initButton(&tft, 160, 290, 280, 70, WHITE, GREEN, BLACK, "SCAN", 3);

  callBtn.drawButton(false);
  scanBtn.drawButton(false);
  if (strcmp(HOME_STATUS, "STANDBY") != 0) {
    endBtn.initButton(&tft, 160, 380, 280, 70, WHITE, BLUE, BLACK, "END", 3);
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

  QrScanBtn.initButton(&tft, 160, 410, 280, 70, WHITE, BLUE, BLACK, "SCAN QR", 3);
  QrScanBtn.drawButton(false);

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

  Text(45, 120, 3, BLACK, "SILAHKAN SCAN");
  Text(65, 160, 3, BLACK, "QR BERIKUT");

  uint8_t qrcodeData[qrcode_getBufferSize(QRcode_Version)];

  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, ECC_HIGH, "tes");

  // Display QR Code on TFT
  uint8_t x0 = 115;
  uint8_t y0 = 220;

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y) == 0) {
        tft.drawPixel(x0 + 2 * x,     y0 + 2 * y, TFT_WHITE);
        // tft.drawPixel(x0 + 2 * x + 1, y0 + 2 * y, TFT_WHITE);
        // tft.drawPixel(x0 + 2 * x,     y0 + 2 * y + 1, TFT_WHITE);
        // tft.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1, TFT_WHITE);
      } else {
        tft.drawPixel(x0 + 1 * x,     y0 + 1 * y, TFT_BLACK);
        // tft.drawPixel(x0 + 2 * x + 1, y0 + 2 * y, TFT_BLACK);
        // tft.drawPixel(x0 + 2 * x,     y0 + 2 * y + 1, TFT_BLACK);
        // tft.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1, TFT_BLACK);
      }
    }
  }

  CURRENT_VIEW = "QR";
}