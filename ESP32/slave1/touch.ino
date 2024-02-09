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
      if(CURRENT_VIEW == "QR") {
        sendJson("qr", currentType);
      }
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
  } else if (CURRENT_VIEW == "QR") {
    Serial.println("HALAMAN QR");
    CURRENT_VIEW = "HOME";
    isChangedView = true;
  }

  //HOME VIEW
  if (CURRENT_VIEW == "HOME") {
    callBtn.press(touched && callBtn.contains(pX, pY));
    NgBtn.press(touched && NgBtn.contains(pX, pY));
    scanBtn.press(touched && scanBtn.contains(pX, pY));
    endBtn.press(touched && endBtn.contains(pX, pY));

    if (readyToChangeView) {
      if (NgBtn.justPressed()) {
        // LAST_VIEW = CURRENT_VIEW;
        // CURRENT_VIEW = "NG";
        routerTwoStack("PARTS");
        isChangedView = true;
      }
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
      sendJson("standby", "none");
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
    QrScanBtn.press(touched && QrScanBtn.contains(pX, pY));


    if (readyToChangeView) {
      if (CHScanBtn.justPressed()) {
        routerTwoStack("CHANGE");
      }

      if (AddScanBtn.justPressed()) {
        RFID_STATUS = "MP";
        sendJson("add_mp");
        routerTwoStack("RFID");
      }

      if (SerScanBtn.justPressed()) {
        routerTwoStack("SERVE");
      }

      if (QrScanBtn.justPressed()) {
        routerTwoStack("QR");
      }
      readyToChangeView = false;
    }
  }

  //CHANGE VIEW
  if (CURRENT_VIEW == "CHANGE") {
    RunChangeBtn.press(touched && RunChangeBtn.contains(pX, pY));
    stateButtonList(touched);

    if (readyToChangeView) {
      if (RunChangeBtn.justPressed()) {
        currentType = "RUNNING";
        sendJson("change", "RUNNING");
        routerThreeStack("RUNNING", "RFID");
      }

      if (MTCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        currentType = "MAINTENANCE";
        sendJson("change", "MAINTENANCE");
        routerThreeStack("MAINTENANCE", "RFID");
      }

      if (MoldCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        currentType = "MOLD";
        sendJson("change", "MOLD");
        routerThreeStack("MOLD", "RFID");
      }

      if (TekCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        currentType = "TEKNISI";
        sendJson("change", "TEKNISI");
        routerThreeStack("TEKNISI", "RFID");
      }

      if (MatCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        currentType = "MATERIAL";
        sendJson("change", "MATERIAL");
        routerThreeStack("MATERIAL", "RFID");
      }

      if (QuaCallBtn.justPressed() && CURRENT_VIEW == "CHANGE") {
        currentType = "QUALITY";
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

  //PARTS VIEW
  if (CURRENT_VIEW == "PARTS") {
    for (int i = 0; i < listItemsLength; i++) {
      ItemButtons[i].press(touched && ItemButtons[i].contains(pX, pY));
      if (readyToChangeView) {
        if (ItemButtons[i].justPressed()) {
          itemId = listItemId[i];
          routerTwoStack("NG");
          isChangedView = true;
        }
        readyToChangeView = false;
      }
    }
  }

  // NG VIEW
  if (CURRENT_VIEW == "NG") {
    submitBtn.press(touched && submitBtn.contains(pX, pY));
    for (int i = 0; i < 5; i++) {
      NgListBtn1[i].press(touched && NgListBtn1[i].contains(pX, pY));
    }

    for (int i = 0; i < 5; i++) {
      NgListBtn2[i].press(touched && NgListBtn2[i].contains(pX, pY));
    }

    if (readyToChangeView) {
      if (submitBtn.justPressed()) {
        String jsonString;
        docSlave1ForNG["state"] = "ng";
        docSlave1ForNG["item_id"] = itemId;
        // docSlave1ForNG["type"] = concatenatedListNG();
        JsonArray jsonArray = docSlave1ForNG.createNestedArray("type");
        for (const auto& id : listNgIds) {
          jsonArray.add(id);
        }

        serializeJson(docSlave1ForNG, jsonString);
        Serial.println(jsonString);
        SerialMaster.println(jsonString.c_str());
      }

      for (int i = 0; i < 5; i++) {
        if (NgListBtn1[i].justPressed()) {
          Serial.println(buttonNgList1[i]);

          uint16_t newColor = isButtonNgList1Pressed[i] ? WHITE : RED;
          setColorAtIndex(i, newColor, 1);

          // Toggle status tombol
          isButtonNgList1Pressed[i] = !isButtonNgList1Pressed[i];
          // docSlave1ForNG[keyNg1[i]] = isButtonNgList1Pressed[i] ? 1 : 0;

          if (isButtonNgList1Pressed[i]) {
            addElementToListNG(valueNg1Ids[i]);
          } else {
            removeElementFromListNG(valueNg1Ids[i]);
          }
        }
      }

      for (int i = 0; i < 5; i++) {
        if (NgListBtn2[i].justPressed()) {
          Serial.println(buttonNgList2[i]);

          uint16_t newColor = isButtonNgList2Pressed[i] ? WHITE : RED;
          setColorAtIndex(i, newColor, 2);

          // Toggle status tombol
          isButtonNgList2Pressed[i] = !isButtonNgList2Pressed[i];
          // docSlave1ForNG[keyNg2[i]] = isButtonNgList2Pressed[i] ? 1 : 0;

          if (isButtonNgList2Pressed[i]) {
            addElementToListNG(valueNg2Ids[i]);
          } else {
            removeElementFromListNG(valueNg2Ids[i]);
          }
        }
      }
      readyToChangeView = false;
    }
  }
  delay(100);
  readyToChangeView = true;
}