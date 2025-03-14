#include "DHT.h"
#include<WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  

  dht.begin();

  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("NetComm 9249");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin("NetComm 9249", "kszadahupf");  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  ThingSpeak.begin(client)
  
  ;

}

void loop() {
  
  float h = dht.readHumidity();
  
  float t = dht.readTemperature();
  
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  ThingSpeak.setField(1, h);
  ThingSpeak.setField(2, t);

  int x= ThingSpeak.writeFields(2873908,"9DTGZPUI26CC6URP");
  if(x==200)
  {
    Serial.println("Channel update successfull");

  }
  else{
    Serial.println("Problem updating the channel");
  }
  
  delay(60000);
}

  
