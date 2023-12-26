void Text(int x, int y, int size, uint16_t color, const char* msg) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(msg);
}

void Header(char* title) {
  backBtn.initButton(&tft, 60, 41, 70, 40, BLACK, BLACK, WHITE, "<BACK", 3);
  backBtn.drawButton(false);
  Text(210, 30, 3, GREEN, title);
  Text(30, 65, 4, YELLOW, "~~~~~~~~~~~");
}

void stateButtonList(bool touched) {
  MTCallBtn.press(touched && MTCallBtn.contains(pX, pY));
  MoldCallBtn.press(touched && MoldCallBtn.contains(pX, pY));
  TekCallBtn.press(touched && TekCallBtn.contains(pX, pY));
  MatCallBtn.press(touched && MatCallBtn.contains(pX, pY));
  QuaCallBtn.press(touched && QuaCallBtn.contains(pX, pY));
}

void ButtonList(int yMT, int yMold, int yTek, int yMat, int yQua, int height) {
  MTCallBtn.initButton(&tft, 160, yMT, 280, height, WHITE, RED, BLACK, "MAINTENANCE", 3);
  MoldCallBtn.initButton(&tft, 160, yMold, 280, height, WHITE, CYAN, BLACK, "MOLD", 3);
  TekCallBtn.initButton(&tft, 160, yTek, 280, height, WHITE, YELLOW, BLACK, "TEKNISI", 3);
  MatCallBtn.initButton(&tft, 160, yMat, 280, height, WHITE, MAGENTA, BLACK, "MATERIAL", 3);
  QuaCallBtn.initButton(&tft, 160, yQua, 280, height, WHITE, BLUE, BLACK, "QUALITY", 3);

  MTCallBtn.drawButton(false);
  MoldCallBtn.drawButton(false);
  TekCallBtn.drawButton(false);
  MatCallBtn.drawButton(false);
  QuaCallBtn.drawButton(false);
}
