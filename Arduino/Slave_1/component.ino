void makeHeader(const char* title) {
  tft.setTextSize(3);

  tft.setTextColor(WHITE);
  tft.setCursor(BTN_BACK_MARGIN, BTN_BACK_MARGIN);
  tft.print(BTN_BACK_LABEL);

  int titleX = tft.width() - ((strlen(title) + 0.2) * 18) - BTN_BACK_MARGIN;
  int titleY = BTN_BACK_MARGIN;
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.setCursor(titleX, titleY);
  tft.print(title);

  int lineX = (tft.width() - 12 * 18) / 2; // 12 is length of string
  int lineY = 30;
  tft.setCursor(lineX, lineY);
  tft.println(F("------------"));
}

void drawButton(int x, int y, int btnWidth, int btnHeight, int color, const char* label, int textColor) {
  tft.fillRoundRect(x, y, btnWidth, btnHeight, 6, color);
  tft.setTextColor(textColor);
  tft.setTextSize(3);
  int labelWidth = strlen(label) * 18; // Menghitung lebar label dalam piksel
  int labelHeight = 12; // Tinggi font adalah 12 piksel
  int labelX = x + (btnWidth - labelWidth) / 2; // Posisi X label di tengah tombol secara horizontal
  int labelY = y + (btnHeight - labelHeight) / 2; // Posisi Y label di tengah tombol secara vertikal
  tft.setCursor(labelX, labelY);
  tft.println(label);
}