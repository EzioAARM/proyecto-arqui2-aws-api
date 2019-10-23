#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//Network parameters
const char* ssid = "Wifiaxel";
const char* password = "123456789";
const int httpsPort = 443;
const char* host = "https://poywhb3qv6.execute-api.us-east-1.amazonaws.com/testing/";
const char fingerPrint[] PROGMEM = "72 D4 00 92 77 37 50 C9 9B A1 38 FA 21 8A 9B FD BA CF CD 49";
const int rounds = 0;

//intervalos
unsigned long prevMilis = 0;
const long interval = 100;

void setup () {
  pinMode(5,INPUT);               //D1 as input
  delay(1000);
  Serial.begin(115200);           //Baudios
  WiFi.mode(WIFI_OFF);            //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);            //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting...");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void loop() {
  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient

  Serial.println(host);

  Serial.printf("Using fingerPrint '%s'\n", fingerPrint);
  httpsClient.setFingerprint(fingerPrint);
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(interval);
  
  Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(interval);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  int adcvalue=analogRead(A0);  //Read Analog value of LDR

  //GET Data
  Link = "modulo-esp?dato=" + rounds;

  Serial.print("requesting URL: ");
  Serial.println(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(2000);  //GET Data at every 2 seconds
}
