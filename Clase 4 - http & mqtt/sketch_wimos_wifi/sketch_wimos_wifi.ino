#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoJson.h> 

char* ssid = "Depto 309";
char* password = "bMQl%Ez3";
char* url = "https://482efd9b-77f2-411a-8ba5-19470d48c68b.mock.pstmn.io/test";
byte mac[6];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("try to connect .. "); 
    delay(1000);
  }

  Serial.print("Connected succefully, ip number: ");
  IPAddress local_ip = WiFi.localIP();
  Serial.println(local_ip);  
 
  macAdressPrint();

  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NETMASK: ");
  Serial.println(subnet);

  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("GATEWAY: ");
  Serial.println(gateway);
}

void loop() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  
  if(http.begin(client, url)) //Iniciar conexión
  {
      Serial.print("[HTTP] POST...\n");

      // ******* Configuracion de JSON *********
      DynamicJsonDocument doc(1024);
      doc["sensor_id"] = "temperatura";
      doc["time"]   = 1351824120;
      doc["temperatura"] = 23.32;
      doc["humedad"] = 90;
      String json;
      serializeJson(doc, json);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(json);
      if(httpResponseCode > 0){
          Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
  
          if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
              String payload = http.getString();   // Obtener respuesta
              Serial.println(payload);   // Mostrar respuesta por serial
          }
      }
      else{
          Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
  
      http.end();
  }
  else{
      Serial.printf("[HTTP} Unable to connect\n");
  }
  
  delay(30000);
}
 

void macAdressPrint()
{
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}
