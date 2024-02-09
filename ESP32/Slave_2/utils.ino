void addElementToListCall(const char* text) {
  if(listCall.size() > 0){
    String compareText = String(text);
    for (int i = 0; i < listCall.size(); i++) {
      if(listCall[i] == compareText) return;
    }
  }

  listCall.push_back(text);
}

String concatenatedListCall(){
  String result;
   for (size_t i = 0; i < listCall.size(); i++) {
    result += listCall[i] + " ";
  }


  return result;
}

void removeElementFromListCall(const char* element){
  String elementToRemove = String(element);

  for (auto it = listCall.begin(); it != listCall.end(); ++it) {
    if (*it == elementToRemove) {
      listCall.erase(it);
      Serial.println("== Hapus");
      break;
    }
  }
  delay(500);
}