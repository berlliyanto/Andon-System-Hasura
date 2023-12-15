int detectButtonTouch(int x, int y, int btnHeight, int btnMargin, int btnQty) {
  for (int i = 0; i < btnQty; i++) {
    if (x >= BTN_START_X && x <= BTN_START_X + BTN_WIDTH && y >= BTN_START_Y + (i * (btnHeight + btnMargin)) && y <= BTN_START_Y + ((i + 1) * btnHeight) + (i * btnMargin)) {
      return i + 1;
    }
  }
  return 0;
}