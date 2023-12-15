static void normalState() {
  TSPoint point = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore the TFT control pins
  pinMode(XM, OUTPUT);
  if (point.z > MINPRESSURE && point.z < MAXPRESSURE) {
    // Scale the touch screen coordinates to match the TFT screen size
    int x = map(point.x, TS_MAXX, TS_MINX, 0, tft.width());
    int y = map(point.y, TS_MINY, TS_MAXY, 0, tft.height());

    boolean isBackPressed = false;
    if (x >= BTN_BACK_MARGIN && x <= BTN_BACK_MARGIN + (strlen(BTN_BACK_LABEL) * 18) && y >= BTN_BACK_MARGIN && y <= 30) {
      isBackPressed = true;
    }

    int buttonClicked = 0;

    if (ACTIVE_SCREEN == "HOME") {
      Serial.println(point.x);
      IS_SERVE = false;
      buttonClicked = detectButtonTouch(x, y, BTN_HOME_HEIGHT, BTN_HOME_MARGIN, BTN_HOME_QTY);

      if (buttonClicked == 1) {
        showCallScreen();
      } else if (buttonClicked == 2) {
        showScanScreen();
      } else if (CURRENT_STATUS != "STANDBY" && buttonClicked == 3) {
        showHomeScreen("");
      }
    } else if (ACTIVE_SCREEN == "CALL") {

      buttonClicked = detectButtonTouch(x, y, BTN_CALL_HEIGHT, BTN_CALL_MARGIN, BTN_CALL_QTY);

      if (isBackPressed) {
        if (IS_SERVE) showScanScreen();
        else showHomeScreen(CURRENT_STATUS);
      } else if (buttonClicked == 1) {
        if (IS_SERVE) {
          RFID_STATUS = "serve_maintenance";
          showRfidScreen();
        } else {
          Serial.println("maintenance");
        }
      } else if (buttonClicked == 2) {
        if (IS_SERVE) {
          RFID_STATUS = "serve_mold";
          showRfidScreen();
        } else {
          Serial.println("mold");
        }
      } else if (buttonClicked == 3) {
        if (IS_SERVE) {
          RFID_STATUS = "serve_technician";
          showRfidScreen();
        } else {
          Serial.println("technician");
        }
      } else if (buttonClicked == 4) {
        if (IS_SERVE) {
          RFID_STATUS = "serve_material";
          showRfidScreen();
        } else {
          Serial.println("material");
        }
      } else if (buttonClicked == 5) {
        if (IS_SERVE) {
          RFID_STATUS = "serve_quality";
          showRfidScreen();
        } else {
          Serial.println("quality");
        }
      }
    } else if (ACTIVE_SCREEN == "SCAN") {
      RFID_STATUS = "";
      buttonClicked = detectButtonTouch(x, y, BTN_SCAN_HEIGHT, BTN_SCAN_MARGIN, BTN_SCAN_QTY);
      if (isBackPressed) {
        showHomeScreen(CURRENT_STATUS);
      } else if (buttonClicked == 1) {
        showChangeScreen();
      } else if (buttonClicked == 2) {
        RFID_STATUS = "add_mp";
        showRfidScreen();
      } else if (buttonClicked == 3) {
        IS_SERVE = true;
        showCallScreen();
      }
    } else if (ACTIVE_SCREEN == "CHANGE") {
      buttonClicked = detectButtonTouch(x, y, BTN_CHANGE_HEIGHT, BTN_CHANGE_MARGIN, BTN_CHANGE_QTY);
      if (isBackPressed) {
        showScanScreen();
      } else if (buttonClicked == 1) {
        RFID_STATUS = "changestatus_running";
        showRfidScreen();
      } else if (buttonClicked == 2) {
        RFID_STATUS = "changestatus_maintenance";
        showRfidScreen();
      } else if (buttonClicked == 3) {
        RFID_STATUS = "changestatus_qdc";
        showRfidScreen();
      } else if (buttonClicked == 4) {
        RFID_STATUS = "changestatus_setting";
        showRfidScreen();
      } else if (buttonClicked == 5) {
        RFID_STATUS = "changestatus_drying";
        showRfidScreen();
      } else if (buttonClicked == 6) {
        RFID_STATUS = "changestatus_trial";
        showRfidScreen();
      }
    } else if (ACTIVE_SCREEN == "RFID") {
      if (RFID_STATUS == "" || isBackPressed) {
        showScanScreen();
      }
    }
  } else if (ACTIVE_SCREEN == "RFID") {
    Serial.println("ini ");
  }
}