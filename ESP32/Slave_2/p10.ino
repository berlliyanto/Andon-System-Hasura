void displayP10() {
  const char* wcCode = "";

  int firstLineLength = strlen(wcCode) + strlen(firstLine);
  int firstLineWidth = firstLineLength * 12;
  char newFirstLine[firstLineLength];
  strcpy(newFirstLine, firstLine);
  strcat(newFirstLine, wcCode);

  int secondLineLength = 0;
  int secondLineWidth = 0;

  if(listCall.size() > 0){
    String concat = concatenatedListCall();
    secondLineLength = concat.length();
    secondLineWidth = secondLineLength * 9;
    if(secondLineLength <= 13){
      secondLineWidth = secondLineLength * 10 + 15;
    }
  }

  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 30) {
    dma_display->fillScreen(0);

    if (lastFirstLineXPos < firstLineWidth) {
      displayFirstRow(newFirstLine);
      lastFirstLineXPos++;
    } else {
      lastFirstLineXPos = 0;
    }

    if (lastSecondLineXPos < secondLineWidth) {
      displaySecondRow();
      lastSecondLineXPos++;
    } else {
      lastSecondLineXPos = 0;
    }

    lastTime = currentTime;
  }
}

void displayFirstRow(char* firstLine) {
  if (strstr(firstLine, "RUNNING")) {
    dma_display->setTextColor(myGREEN);
  } else if (strstr(firstLine, "MAINTENANCE")) {
    dma_display->setTextColor(myRED);
  } else if (strstr(firstLine, "QDC")) {
    dma_display->setTextColor(myCYAN);
  } else if (strstr(firstLine, "SETTING")) {
    dma_display->setTextColor(myYELLOW);
  } else if (strstr(firstLine, "DRYING")) {
    dma_display->setTextColor(myPURPLE);
  } else if (strstr(firstLine, "TRIAL")) {
    dma_display->setTextColor(myBLUE);
  } else if (strstr(firstLine, "STANDBY")) {
    dma_display->setTextColor(myBROWN);
  } else {
    dma_display->setTextColor(myWHITE);
  }
  dma_display->setCursor(32 - lastFirstLineXPos, 7);
  dma_display->print(firstLine);
}

void displaySecondRow() {
  
  if (listCall.size() > 0) {
    dma_display->setCursor(32 - lastSecondLineXPos, 16);
    for (int i = 0; i < listCall.size(); i++) {
      if (listCall[i] == "MAINTENANCE") {
        dma_display->setTextColor(myRED);
        dma_display->print("MAINTENANCE ");
      } else if (listCall[i] == "MOLD") {
        dma_display->setTextColor(myCYAN);
        dma_display->print("MOLD ");
      } else if (listCall[i] == "TEKNISI") {
        dma_display->setTextColor(myYELLOW);
        dma_display->print("TEKNISI ");
      } else if (listCall[i] == "MATERIAL") {
        dma_display->setTextColor(myPURPLE);
        dma_display->print("MATERIAL ");
      } else if (listCall[i] == "QUALITY") {
        dma_display->setTextColor(myBLUE);
        dma_display->print("QUALITY ");
      } else {
        dma_display->setTextColor(myWHITE);
      }
    }
  }
}