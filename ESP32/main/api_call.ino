String makeBody(String state, String type, String rfid = "") {
  String jsonString;
  String isAndon = "&is_andon=1";
  String WCD = "work_center_code=" + wcCode;

  if (state == "call") {
    jsonString = WCD + "&type=" + type + "&is_calling=1" + isAndon;
  } else if (state == "end") {
    jsonString = WCD + isAndon;
  } else if (state == "add_mp") {
    jsonString = WCD + "&rfid=" + rfid + isAndon;
  } else if (state == "serve") {
    jsonString = WCD + "&type=" + type + "&is_calling=0" + isAndon + "&rfid=" + rfid;
  } else if (state == "change") {
    jsonString = WCD + "&status=" + type + isAndon + "&rfid=" + rfid;
  } else if (state == "shot") {
    jsonString = WCD;
  }

  return jsonString;
}

void getAPI(String url, String host) {
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Connection: keep-alive");
  client.println();
}

void postAPI(String url, String host, String body) {
  Serial.print("body : ");
  Serial.println(body.length());
  Serial.println(body);
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Accept: application/json");
  client.println("Connection: keep-alive");
  client.print("Content-Length: ");
  client.println(body.length(), DEC);
  client.println();
  client.println(body);
  client.flush();
}

void connectClient(byte server[], int port) {
  if (client.connect(server, port)) {
    Serial.println("Connected to Server");
    delay(1000);
    if (initState) {
      getAPI("/api/iotreport/andonmonitoring?is_andon=1&wc=1WCINJ-008", hostDev);
      delay(500);
      initState = false;
    }
  } else {
    Serial.println("Connection Failed to Server");
  }
}

void receiveResponse() {
  if (client) client.setTimeout(5000);
  int len = client.available();
  if (len > 0) {
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = client.readBytes(buffer, static_cast<size_t>(min(static_cast<size_t>(len), static_cast<size_t>(sizeof(buffer) - 1))));

    if (strstr(buffer, "HTTP/1.1 200")) {
      Serial.println("RESPONSE 200 OK");
      char *responseStart = strstr(buffer, "{\"status\":");
      if (holdingJson.indexOf("change") != -1 || holdingJson.indexOf("standby") != -1 || holdingJson == "") {
        if (responseStart != nullptr) {
          if (strlen(responseStart) >= strlen("{\"status\":")) {
            char *response = responseStart;
            if (strstr(response, "RUNNING")) holdingJson = "{\"state\":\"change\",\"type\":\"RUNNING\"}";
            else if (strstr(response, "STANDBY")) holdingJson = "{\"state\":\"standby\",\"type\":\"none\"}";
            else if (strstr(response, "MAINTENANCE")) holdingJson = "{\"state\":\"change\",\"type\":\"MAINTENANCE\"}";
            else if (strstr(response, "QDC")) holdingJson = "{\"state\":\"change\",\"type\":\"MOLD\"}";
            else if (strstr(response, "SETTING")) holdingJson = "{\"state\":\"change\",\"type\":\"TEKNISI\"}";
            else if (strstr(response, "DRYING")) holdingJson = "{\"state\":\"change\",\"type\":\"MATERIAL\"}";
            else if (strstr(response, "TRIAL")) holdingJson = "{\"state\":\"change\",\"type\":\"QUALITY\"}";
          }
        } else {
          Serial.println("Incomplete JSON response");
        }
      }
      writeToSerial2(holdingJson);
      writeToSerial1(holdingJson);
    } else {
      sendLoadingState("isNotLoading");
      Serial.println("RESPONSE NOT OK");
    }

    Serial.println(buffer);

    while (client.available()) {
      client.read();
    }
  }

  if (!client.connected()) {
    Serial.println();
    Serial.print("Disconnecting -> ");
    client.stop();

    connectClient(serverDev, port);
  }
}
