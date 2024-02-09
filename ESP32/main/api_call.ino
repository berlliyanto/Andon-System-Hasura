//FUNCTION TO GENERATE BODY FOR POST REQUEST
String makeBody(String state, String type, String rfid = "", String itemId = "") {
  DynamicJsonDocument jsonBody(512);
  String jsonString;
  // String isAndon = "&is_andon=1";
  // String WCD = "work_center_code=" + wcCode;

  if (state == "call") {
    // jsonString = WCD + "&type=" + type + "&is_calling=1" + isAndon;
    jsonBody["work_center_code"] = wcCode;
    jsonBody["type"] = type;
    jsonBody["is_calling"] = 1;
    jsonBody["is_andon"] = 1;
  } else if (state == "end") {
    // jsonString = WCD + isAndon;
    jsonBody["work_center_code"] = wcCode;
    jsonBody["is_andon"] = 1;
  } else if (state == "add_mp") {
    // jsonString = WCD + "&rfid=" + rfid + isAndon;
    jsonBody["work_center_code"] = wcCode;
    jsonBody["rfid"] = rfid;
    jsonBody["is_andon"] = 1;
  } else if (state == "serve") {
    // jsonString = WCD + "&type=" + type + "&is_calling=0" + isAndon + "&rfid=" + rfid;
    jsonBody["work_center_code"] = wcCode;
    jsonBody["type"] = type;
    jsonBody["is_calling"] = 0;
    jsonBody["rfid"] = rfid;
    jsonBody["is_andon"] = 1;
  } else if (state == "change") {
    // jsonString = WCD + "&status=" + type + isAndon + "&rfid=" + rfid;
    jsonBody["work_center_code"] = wcCode;
    jsonBody["status"] = type;
    jsonBody["rfid"] = rfid;
    jsonBody["is_andon"] = 1;
  } else if (state == "shot") {
    // jsonString = WCD;
    jsonBody["work_center_code"] = wcCode;
  } else if (state == "ng") {
    // jsonString = WCD + "&item_id=" + itemId + "&ng_ids=" + type + "&cav_number=1";
    DynamicJsonDocument ngIds(200);
    deserializeJson(ngIds, type);
    jsonBody["work_center_code"] = wcCode;
    jsonBody["item_id"] = itemId;
    jsonBody["ng_ids"] = ngIds;
    // jsonBody["cav_number"] = 1;
  }

  serializeJson(jsonBody, jsonString);
  // Serial.println(jsonString);
  return jsonString;
}

//FUNCTION TO GET REQUEST
void getAPI(String url, String host) {
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Connection: keep-alive");
  // client.println("Accept: application/json");
  client.println();
  requestTimer = millis() + REQUEST_TIMEOUT;
  waitingResponse = true;
}

// FUNCTION TO POST REQUEST
void postAPI(String url, String host, String body) {
  sendLoadingState("isLoading");  // SEND LOADING STATE TRUE
  delay(500);
  Serial.print("body : ");
  Serial.println(body);
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Content-Type: application/json");
  client.println("Accept: application/json");
  client.println("Connection: keep-alive");
  client.print("Content-Length: ");
  client.println(body.length(), DEC);
  client.println();
  client.println(body);
  requestTimer = millis() + REQUEST_TIMEOUT;  // START THE TIMER
  waitingResponse = true;                     // CONDITION OF RESPONSE
}

//CONNECTING ETHERNET TO CLIENT
void connectClient(byte server[], int port) {
  if (client.connect(server, port)) {
    Serial.println("Connected to Server");
    delay(1000);
    if (onInit) {
      // getAPI("/api/workcenterstatus/1WCINJ-008", hostDev);
      getAPI("/api/iotreport/andonmonitoring?is_andon=1&wc=" + wcCode, hostDev);
      // getAPI("/api/timeserver", host);
      // getAPI("/api/tes", hostTes);
      delay(500);
      onInit = false;
    }
  } else {
    Serial.println("Connection Failed to Server");
  }
}

// GET THE RESPONSE FROM API
void receiveResponse() {
  // client.setTimeout(5000);  // TIMEOUT USELESS LMAO
  int len = client.available();
  if (len > 0) {
    char buffer[4096];  // ADJUST BUFFER FOR THE BIGGER RESPONSE
    memset(buffer, 0, sizeof(buffer));

    // int bytesRead = client.readBytes(buffer, static_cast<size_t>(min(static_cast<size_t>(len), static_cast<size_t>(sizeof(buffer) - 1))));
    // Serial.println(bytesRead);

    int bytesRead = client.readBytes(buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';

    //CHECK RESPONSE
    if (strstr(buffer, "HTTP/1.1 200")) {
      bodyResponse.clear();
      delay(500);
      // char* responseStart = strstr(buffer, "{\"status\":");
      if (holdingJson.indexOf("change") != -1 || holdingJson.indexOf("standby") != -1 || 
      holdingJson.indexOf("qr") != -1 || holdingJson.indexOf("ng") != -1 || holdingJson == "") {
        // if (responseStart != nullptr) {
        //   if (strlen(responseStart) >= strlen("{\"status\":")) {
        //     char *response = responseStart;
        //     //MANUAL CHECKING FROM RESPONSE AND MANUAL SET DATA
        // if (strstr(response, "running")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"RUNNING\"}";
        //   currentStatus = "RUNNING";
        // } else if (strstr(response, "standby")) {
        // holdingJson = "{\"state\":\"standby\",\"type\":\"none\"}";
        // currentStatus = "STANDBY";
        // } else if (strstr(response, "maintenance")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"MAINTENANCE\"}";
        //   currentStatus = "MAINTENANCE";
        // } else if (strstr(response, "qdc")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"MOLD\"}";
        //   currentStatus = "QDC";
        // } else if (strstr(response, "setting")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"TEKNISI\"}";
        //   currentStatus = "SETTING";
        // } else if (strstr(response, "drying")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"MATERIAL\"}";
        //   currentStatus = "DRYING";
        // } else if (strstr(response, "trial")) {
        //   holdingJson = "{\"state\":\"change\",\"type\":\"QUALITY\"}";
        //   currentStatus = "TRIAL";
        // }

        //   }
        // } else {
        //   Serial.println("Incomplete JSON response");
        // }
        //------------USE THIS TO ONLY GET RESPONSE BODY FROM ETHERNET CLIENT-------------//
        char* bodyStart = strstr(buffer, "\r\n\r\n");
        Serial.println(bodyStart);
        if (bodyStart != nullptr) {
          bodyStart += 4;
          char* body = strstr(bodyStart, "{\"data\":{\"work_center_id\":");  // VARIABEL MENAMPUNG BODY
          String jsonBody = String(body);
          deserializeJson(bodyResponse, jsonBody);
          String jsonString = "";
          const char* status = "";
          char uppercaseStatus[100];
          tempJson["state"] = "change";

          if (bodyResponse.containsKey("data")) {

            if (bodyResponse["data"].containsKey("status")) {
              status = bodyResponse["data"]["status"];
              Serial.println("Original status: " + String(status));            // Output status sebelum diubah
              strcpy(uppercaseStatus, status);                                 // Salin status ke string baru
              toUpperCase(uppercaseStatus);                                    // Ubah status menjadi huruf kapital
              Serial.println("Uppercase status: " + String(uppercaseStatus));  // Output status setelah diubah menjadi huruf kapital
              tempJson["type"] = uppercaseStatus;
            }

            if (bodyResponse["data"].containsKey("work_center_id")) {
              tempJson["work_center_id"] = bodyResponse["data"]["work_center_id"];
              workCenterId = bodyResponse["data"]["work_center_id"];
            }

            if (bodyResponse["data"].containsKey("work_order_no")) {
              if (bodyResponse["data"]["work_order_no"] != nullptr) {
                workOrderNo = bodyResponse["data"]["work_order_no"];
                tempJson["work_order_no"] = bodyResponse["data"]["work_order_no"];
              }
            }

            if (bodyResponse["data"].containsKey("products")) tempJson["items"] = bodyResponse["data"]["products"];
          }


          serializeJson(tempJson, jsonString);
          delay(100);
          holdingJson = jsonString;


          if (strcmp(status, "standby") == 0) {
            currentStatus = "STANDBY";
          } else if (strcmp(status, "running") == 0) {
            currentStatus = "RUNNING";
          } else if (strcmp(status, "maintenance") == 0) {
            currentStatus = "MAINTENANCE";
          } else if (strcmp(status, "QDC") == 0) {
            currentStatus = "MOLD";
          } else if (strcmp(status, "setting") == 0) {
            currentStatus = "TEKNISI";
          } else if (strcmp(status, "drying") == 0) {
            currentStatus = "MATERIAL";
          } else if (strcmp(status, "trial") == 0) {
            currentStatus = "QUALITY";
          }
          Serial.print("Holding Json : ");
          Serial.println(holdingJson);
        }
      }
      waitingResponse = false;


      // if (!isListenStatus && currentStatus == oldStatus) {
      writeToSerial2(holdingJson);  // SEND JSON TO SLAVE 2
      writeToSerial1(holdingJson);  // SEND JSON TO SLAVE 1
      // }
    } else {
      sendLoadingState("isNotLoading");  // SEND LOADING STATE FALSE
      isListenStatus = false;
      Serial.println("RESPONSE NOT OK");
    }

    // Serial.println(buffer);

    while (client.available()) {
      client.read();
    }
  }

  if (!client.connected()) {
    Serial.println();
    Serial.print("Disconnecting -> ");
    client.stop();

    connectClient(serverDev, portDev);
  }
}

void toUpperCase(char* str) {
  while (*str) {
    *str = toupper(*str);
    str++;
  }
}

void listenStatus() {
  // if(oldStatus == currentStatus){
  //   isListenStatus = false;
  //   Serial.println("asdsadsa");
  // }
  if (isListenStatus) {
    getAPI("/api/iotreport/andonmonitoring?is_andon=1&wc=1WCINJ-009", hostDev);
    Serial.println("isListening");
    // oldStatus = currentStatus;
  }
}
