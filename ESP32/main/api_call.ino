void getAPI(String url, String host) {
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Connection: close");
  client.println();
}

void postAPI(String url, String host, String body){
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + host);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Accept: application/json");
  client.println("Connection: close");
  client.println("Content-Length: " + body.length());
  client.println(); 
  client.println(body);
}

bool connectClient(byte server[], int port) {
  if (client.connect(server, port)) {
    isClientConnected = true;
    Serial.println("Connected to Server");
    delay(1000);
    return true;
  } else {
    Serial.println("Connection Failed to Server");
    return false;
  }
}

void receiveResponse(){
  int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    if (printWebData) {
      Serial.write(buffer, len);
    }
  }

  if (!client.connected()) {
    isClientConnected = false; //main.ino
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    isClientConnected = connectClient(serverERP, portERP);
  }
}