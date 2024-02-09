void setColorAtIndex(int index, uint16_t color, int row) {
  if (index >= 0 && index < 5) {
    if (row == 1) {
      colorButtonNgList1[index] = color;
    } else if (row == 2) {
      colorButtonNgList2[index] = color;
    }

    isChangedView = true;
  } else {
    Serial.println("Invalid index.");
  }
}

void addElementToListNG(const char* text) {
  if(listNgIds.size() > 0){
    String compareText = String(text);
    for (int i = 0; i < listNgIds.size(); i++) {
      if(listNgIds[i] == compareText) return;
    }
  }

  listNgIds.push_back(text);
}

String concatenatedListNG(){
  String result;
   for (size_t i = 0; i < listNgIds.size(); i++) {
    result += "ng_ids=" + listNgIds[i] + "&";
  }

  return result;
}

void removeElementFromListNG(const char* element){
  String elementToRemove = String(element);

  for (auto it = listNgIds.begin(); it != listNgIds.end(); ++it) {
    if (*it == elementToRemove) {
      listNgIds.erase(it);
      Serial.println("== Hapus");
      break;
    }
  }
  delay(500);
}

void addElementToListItemsId(const char* text) {
  if(listItemId.size() > 0){
    String compareText = String(text);
    for (int i = 0; i < listItemId.size(); i++) {
      if(listItemId[i] == compareText) return;
    }
  }

  listItemId.push_back(text);
}
