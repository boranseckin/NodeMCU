/*
 * NODEMCU Station
 */

#include <ESP8266WiFi.h>

String rx;
String inputPass;
int inputSSID;
bool ifScan;

void setup() {
  
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  ifScan = false;
  delay(100);

}

void selectSSID() {

  Serial.println();
  Serial.println("Enter the number of the SSID...");

  while(Serial.available() <= 0) {
    delay(500);
  }
  
  if(Serial.available() > 0) {

    inputSSID = Serial.parseInt();
    Serial.print(WiFi.SSID(inputSSID));
    Serial.println(" selected!");
    getPass();
    
  }
  
}

void getPass() {

  Serial.println();
  Serial.print("Enter the password for ");
  Serial.print(WiFi.SSID(inputSSID));
  Serial.println("...");

  while(Serial.available() <= 0) {
    delay(500);
  }
  
  if(Serial.available() > 0) {
    
    inputPass = Serial.readString();
    Serial.println(inputPass);
    connect(inputSSID, inputPass);

  }
  
}

void connect(int ssidNumber, String pass) {

  WiFi.begin(WiFi.SSID(ssidNumber), pass);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(WiFi.SSID(ssidNumber));
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.printf("Connected!");
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mac Address: ");
  Serial.printf(WiFi.macAddress().c_str());
  Serial.println();
  
}

void loop() {

  if(Serial.available() > 0) {

    rx = Serial.readString();
    
  }

  if(rx == "dc") {

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println();
    Serial.println("Disconnected!");
    WiFi.disconnect();
    ifScan = false;
    delay(100);
        
  }

  if(rx == "info") {

    Serial.println();

    if(WiFi.status() == WL_CONNECTED) {

      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("Mac Address: ");
      Serial.printf(WiFi.macAddress().c_str());
          
    } else if(WiFi.status() == WL_NO_SSID_AVAIL) {
    
      Serial.println("The network not found!");
      
    } else if(WiFi.status() == WL_CONNECT_FAILED) {
    
      Serial.println("The password is incorrect!");
      
    } else if(WiFi.status() == WL_IDLE_STATUS) {
    
      Serial.println("No current connection is available!");
      
    } else {
    
      Serial.println("Unknown error!");
      
    }

    Serial.println();
    
  }

  if(rx == "scan") {
    
    Serial.println();
    Serial.print("Scan started ... ");
    int n = WiFi.scanNetworks();
    Serial.print(n);
    Serial.println(" network(s) found:");
    Serial.println();
    for (int i = 0; i < n; i++)
    {
      Serial.print(" ");
      Serial.print(i);
      Serial.print(" -> ");
      Serial.println(WiFi.SSID(i));
    }
    Serial.println();
    Serial.print("Scan completed!");
    ifScan = true;
    Serial.println();
    
  }

  if(rx == "connect") {

    if(ifScan == true) {
    
      selectSSID();

    } else {

      Serial.println();
      Serial.println("Use 'scan' before connect!");
      
    }
    
  }

  rx = "";

}
