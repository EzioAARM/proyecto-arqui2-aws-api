#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//parametros de red wifi
const char* ssid = "Wifiaxel";
const char* password = "123456789";

//intervalos
unsigned long prevMilis = 0;
const long interval = 100;

int datoGuardar = 0;
int datoEnviar = 0;
int dato = 0;
int cont = 0;
int inser = 0;
int hecho = 0;
boolean enviado = false;
boolean paso255 = false;

void setup () {
  //PMOD CONFIG
  pinMode(16,INPUT);//D0
  pinMode(5,INPUT);//D1
  pinMode(4,INPUT);//D2
  pinMode(0,INPUT);//D3
  pinMode(2,INPUT);//D4
  pinMode(14,INPUT);//D5
  pinMode(12,INPUT);//D6
  pinMode(13,INPUT);//D7
  pinMode(15, OUTPUT);//D8
  digitalWrite(15, LOW); 
  //Baudios
  Serial.begin(115200);
  //conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print("Estableciendo una conexion..\n");
  }
  digitalWrite(15, HIGH);
  Serial.print("Conexion establecida!\n");

  HTTPClient http2;  
    http2.begin("ec2-54-227-0-198.compute-1.amazonaws.com:3000/1");
    int httpCode2 = http2.GET();                                                                  
    if (httpCode2 > 0) { //Check the returning code
      String payload2 = http2.getString();   //Get the request response payload
      Serial.println(payload2);             //Print the response payload
    }
    http2.end();   //Close connection
}

void loop() {
  unsigned long cMilis = millis();
  if(((datoGuardar == datoEnviar && cMilis - prevMilis >= interval) || datoEnviar == 255) && !enviado){
    prevMilis = cMilis;
    HTTPClient http;  //Declare an object of class HTTPClient
    String cadenaEnviar = String(datoEnviar);
    http.begin("http://ec2-54-227-0-198.compute-1.amazonaws.com:3000/?dato=" + cadenaEnviar);  //Direccion url de la instancia de amazon
    int httpCode = http.GET();                                                                  //se hace la peticion GET
    if (httpCode > 0) { //Check the returning code
      Serial.println(httpCode);
      String payload = http.getString();   
      Serial.println(payload);             
    }
    http.end();
    enviado = true;
  } else {
    enviado = false;
    cont = digitalRead(16);
    if (cont != HIGH) {
      dato = dato + 1;
    }
    cont = digitalRead(5);
    if (cont != HIGH) {
      dato = dato + 2;
    }
    cont = digitalRead(4);
    if (cont != HIGH) {
      dato = dato + 4;
    }
    cont = digitalRead(0);
    if (cont != HIGH) {
      dato = dato + 8;
    }
    cont = digitalRead(2);
    if (cont != HIGH) {
      dato = dato + 16;
    }
    cont = digitalRead(14);
    if (cont != HIGH) {
      dato = dato + 32;
    }
    cont = digitalRead(12);
    if (cont != HIGH) {
      dato = dato + 64;
    }
    cont = digitalRead(13);
    if (cont != HIGH) {
      dato = dato + 128;
    }
    datoGuardar = dato;
    if (datoGuardar != datoEnviar) {
      datoEnviar = datoGuardar;
    } else {
      enviado = true;
    }
    dato = 0;
  }
}
