#include <ESP8266WiFi.h>

char* ssid = "IoTNetWork";
char* password = "desarrollo123";
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
  // put your main code here, to run repeatedly:
  //listenerNetworks();
  delay(1000);
}

void listenerNetworks()
{
  Serial.println("** Scan Networks **");
  byte numSsid = WiFi.scanNetworks();

  Serial.print("Number of available WiFi networks discovered:");
  Serial.println(numSsid);

  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }
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
