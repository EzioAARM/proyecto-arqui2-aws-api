#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//parametros de red wifi
const char* ssid = "Wifiaxel";
const char* password = "123456789";

//intervalos
unsigned long prevMilis = 0;
const long interval = 1000;

int dato = 0;
int cont = 0;
int inser = 0;
int hecho = 0;

void setup () {
  //PMOD CONFIG
  pinMode(5,INPUT);//D1
  //Baudios
  Serial.begin(115200);
  //conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Estableciendo una conexion..\n");
  }
  Serial.print("Conexion establecida!\n");

  HTTPClient http2;  
    http2.begin("http://ec2-18-188-175-99.us-east-2.compute.amazonaws.com:3000/");
    int httpCode2 = http2.GET();                                                                  
    if (httpCode2 > 0) { //Check the returning code
      String payload2 = http2.getString();   //Get the request response payload
      Serial.println(payload2);             //Print the response payload
    }
    http2.end();   //Close connection
  
}

void loop() {
  unsigned long cMilis = millis();
  if(cMilis - prevMilis >= interval){
    prevMilis = cMilis;
    cont = digitalRead(5);
    if (cont == HIGH) {
      HTTPClient http;  //Declare an object of class HTTPClient
      http.begin("http://ec2-18-188-175-99.us-east-2.compute.amazonaws.com:3000/" + datoNuevo);  //Direccion url de la instancia de amazon
      int httpCode = http.GET();                                                                  //se hace la peticion GET
      if (httpCode > 0) { //Check the returning code
        Serial.println(httpCode);
        String payload = http.getString();   
        Serial.println(payload);             
      }
      http.end();
    } else {
      Serial.println("no se guardo nada");  
    }
  }
}
