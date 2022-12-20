/*
fuente: https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/
*/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <WiFiUdp.h>
#include <ArduinoJson.h> 

char* ssid = "Depto 309";
char* password = "bMQl%Ez3";
char* url = "http://192.168.100.30:5000/wemostime";
const long utcOffsetInSeconds = -10800; //UTC -4 Santiago. 4*60*60, pero como es horario verano es -3 => -3*60*60
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.shoa.cl", utcOffsetInSeconds);

String formattedDate;

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    Serial.print(".");
  }

  timeClient.begin();
}

void loop() {
  HTTPClient http;
  WiFiClient client;
  timeClient.update();

  formattedDate = timeClient.getFormattedTime(); 
  Serial.println(formattedDate);

  if(http.begin(client, url)) //Iniciar conexión
    {
        Serial.print("[HTTP] POST...\n");

        // ******* Configuracion de JSON *********
        DynamicJsonDocument doc(1024);
        doc["time"] = formattedDate;
         
        String json;
        serializeJson(doc, json);
        Serial.println(json.c_str());
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(json.c_str());
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
     
    delay(1000);
}