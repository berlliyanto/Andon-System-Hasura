bool getTouch(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pX = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    pY = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

void touchOperation() {
  bool touched = getTouch();

  //BACK BUTTON
  if (CURRENT_VIEW != "HOME") {
    backBtn.press(touched && backBtn.contains(pX, pY));
    char* TEMPORARY_VIEW = "";
    if (backBtn.justPressed()) {
      if (LAST_VIEW3 != "") {
        TEMPORARY_VIEW = LAST_VIEW;
        LAST_VIEW = LAST_VIEW2;
        LAST_VIEW2 = LAST_VIEW3;
        LAST_VIEW3 = "";
      } else if (LAST_VIEW2 != "" && LAST_VIEW3 == "") {
        TEMPORARY_VIEW = LAST_VIEW;
        LAST_VIEW = LAST_VIEW2;
        LAST_VIEW2 = "";
      } else {
        TEMPORARY_VIEW = LAST_VIEW;
      }
      CURRENT_VIEW = TEMPORARY_VIEW;
      isChangedView = true;
    }
  }

  //HOME VIEW
  if (CURRENT_VIEW == "HOME") {
    callBtn.press(touched && callBtn.contains(pX, pY));
    scanBtn.press(touched && scanBtn.contains(pX, pY));
    endBtn.press(touched && endBtn.contains(pX, pY));

    if (readyToChangeView) {
      if (callBtn.justPressed()) {
        LAST_VIEW = CURRENT_VIEW;
        CURRENT_VIEW = "CALL";
        isChangedView = true;
      }

      if (scanBtn.justPressed()) {
        routerTwoStack("SCAN");
      }
      readyToChangeView = false;
    }

    if (endBtn.justPressed()) {
      // Serial.println("END");
      sendJson("shot", "none", 1);
    }
  }

  //CALL VIEW
  if (CURRENT_VIEW == "CALL") {
    stateButtonList(touched);

    if (readyToChangeView) {
      if (MTCallBtn.justPressed()) {
        sendJson("call", "MAINTENANCE");
      }

      if (MoldCallBtn.justPressed()) {
        sendJson("call", "MOLD");
      }

      if (TekCallBtn.justPressed()) {
        sendJson("call", "TEKNISI");
      }

      if (MatCallBtn.justPressed()) {
        sendJson("call", "MATERIAL");
      }

      if (QuaCallBtn.justPressed()) {
        sendJson("call", "QUALITY");
      }
      readyToChangeView = false;
    }
  }

  //SCAN VIEW
  if (CURRENT_VIEW == "SCAN") {
    CHScanBtn.press(touched && CHScanBtn.contains(pX, pY));
    AddScanBtn.press(touched && AddScanBtn.contains(pX, pY));
    SerScanBtn.press(touched && SerScanBtn.contains(pX, pY));

    if (readyToChangeView) {
      if (CHScanBtn.justPressed()) {
        routerTwoStack("CHANGE");
      }

      if (AddScanBtn.justPressed()) {
        RFID_STATUS = "MP";
        routerTwoStack("RFID");
      }

      if (SerScanBtn.justPressed()) {
        routerTwoStack("SERVE");
      }
      readyToChangeView = false;
    }
  }

  //CHANGE VIEW
  if (CURRENT_VIEW == "CHANGE") {
    RunChangeBtn.press(touched && RunChangeBtn.contains(pX, pY));
    stateButtonList(touched);

    if (readyToChangeView) {
      if (RunChangeBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "RUNNING");
        routerThreeStack("RUNNING", "RFID");
      }

      if (MTCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "MAINTENANCE");
        routerThreeStack("MAINTENANCE", "RFID");
      }

      if (MoldCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "MOLD");
        routerThreeStack("MOLD", "RFID");
      }

      if (TekCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "TEKNISI");
        routerThreeStack("TEKNISI", "RFID");
      }

      if (MatCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "MATERIAL");
        routerThreeStack("MATERIAL", "RFID");
      }

      if (QuaCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        sendJson("change", "QUALITY");
        routerThreeStack("QUALITY", "RFID");
      }
      readyToChangeView = false;
    }
  }

  //SERVE VIEW
  if (CURRENT_VIEW == "SERVE") {
    stateButtonList(touched);

    if (readyToChangeView) {
      if (MTCallBtn.justPressed()) {
        sendJson("serve", "MAINTENANCE");
        routerThreeStack("MAINTENANCE", "RFID");
      }

      if (MoldCallBtn.justPressed()) {
        sendJson("serve", "MOLD");
        routerThreeStack("MOLD", "RFID");
      }

      if (TekCallBtn.justPressed()) {
        sendJson("serve", "TEKNISI");
        routerThreeStack("TEKNISI", "RFID");
      }

      if (MatCallBtn.justPressed()) {
        sendJson("serve", "MATERIAL");
        routerThreeStack("MATERIAL", "RFID");
      }

      if (QuaCallBtn.justPressed()) {
        sendJson("serve", "QUALITY");
        routerThreeStack("QUALITY", "RFID");
      }
      readyToChangeView = false;
    }
  }
  delay(100);
  readyToChangeView = true;
}