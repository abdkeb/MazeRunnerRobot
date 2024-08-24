#ifndef DEBUG_H
#define DEBUG_H

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  // WebSerial.println(d);
  // if (d == "ON"){
  //   digitalWrite(LED, HIGH);
  // }
  // if (d=="OFF"){
  //   digitalWrite(LED, LOW);
  // }
}

void debugSetup() {
  got_ip = false;
  pinMode(LED, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  IPAddress localIP = WiFi.localIP();
  Serial.println(localIP);
  Serial.println();
  
  WebSerial.begin(&server);
  // WebSerial.msgCallback(recvMsg);
  server.begin();
}

void getIPAddress(){
   if( !got_ip){
    Serial.println("started loop");
    got_ip = true;
    for(int i = 0; i < 10; i++){
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    }
  }
}



#endif // DEBUG_H
