#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

char* ssid = "Depto 309";
char* password = "bMQl%Ez3";

const char* mqttServer = "test.mosquitto.org";
int port = 1883;
const char* subscribeTopic = "pucv/iot/m6/p3/g4";

WiFiClient espClient;
PubSubClient client(espClient);
char clientId[50];

void setup(){
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    delay(500);
    
    Serial.print("Conectando a WiFi ");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("conectado");

    //***** Obtener direccion IP asignada *******
    IPAddress ip;
    ip = WiFi.localIP();
    Serial.print("Direccion IP: ");
    Serial.println(ip);

    //***** Configurando MQTT Client ************
    client.setServer(mqttServer, port);
    client.setCallback(callback);

}

void loop(){
    delay(10);
    if (!client.connected())
    {
        mqttReconnect();
    }
    client.loop();
}

void callback(char* topic, byte* message, unsigned int length){
    Serial.print("Mensaje recibido en el topico: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String stMessage;
    client.publish(subscribeTopic,"{'data': 'world'}");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        stMessage += (char)message[i];
    }
    Serial.println();
}

void mqttReconnect(){
    while (!client.connected()){
        Serial.print("Intentando conectar al broker MQTT con cliente ");
        long r = random(1000);
        sprintf(clientId, "clientId-%ld", r);
        if (client.connect(clientId)){
            Serial.print(clientId);
            Serial.println(".......conectado");
            client.subscribe(subscribeTopic);
        }
        else{
            Serial.print("falló, rc=");
            Serial.print(client.state());
            Serial.println(".....Reintentando en 5 segundos");
            delay(5000);
        }
    }
}
