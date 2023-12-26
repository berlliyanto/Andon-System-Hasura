void routes() {

  if (isChangedView) {
    if (CURRENT_VIEW == "HOME") {
      HomeView("HOME");
    } else if (CURRENT_VIEW == "CALL") {
      CallView("CALL");
    } else if (CURRENT_VIEW == "SCAN") {
      ScanView("SCAN");
    } else if (CURRENT_VIEW == "CHANGE") {
      ChangeView("CHANGE");
    } else if (CURRENT_VIEW == "SERVE") {
      ServeView("SERVE");
    } else if (CURRENT_VIEW == "RFID") {
      RFIDView("TAP");
    }

    isChangedView = false;
  }
}

void routerTwoStack(char* currentView) {
  LAST_VIEW2 = LAST_VIEW;
  LAST_VIEW = CURRENT_VIEW;
  CURRENT_VIEW = currentView;
  isChangedView = true;
}

void routerThreeStack(char* rfidStatus, char* currentView) {
  RFID_STATUS = rfidStatus;
  LAST_VIEW3 = LAST_VIEW2;
  LAST_VIEW2 = LAST_VIEW;
  LAST_VIEW = CURRENT_VIEW;
  CURRENT_VIEW = currentView;
  isChangedView = true;
}