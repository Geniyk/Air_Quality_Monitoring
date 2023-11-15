#define BLYNK_TEMPLATE_ID "TMPL3RyI6W0Az" //YOUR BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME "Air Quality" //YOUR BLYNK_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN "ysCfDQtWW_CIZzZHVEQ6J_NFSYJx2lxB" //YOUR BLYNK_AUTH_TOKEN

char auth[] = "ysCfDQtWW_CIZzZHVEQ6J_NFSYJx2lxB";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*****"; //YOUR HOTSPOT NAME
char pass[] = "******"; //YOUR HOTSPOT PASSWORD

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//GPS Module Settings
//GPS Module RX pin to ESP32 17
//GPS Module TX pin to ESP32 16
#include <TinyGPSPlus.h> 

TinyGPSPlus gps;

BlynkTimer timer;

int gas=32;
int sensorThreshold=100;

#define INTERVAL 1000L

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println(F("INVALID"));
  }
}
void sendGps()
{
  
    while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
  
  //get latitude and longitude
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  
  //get Gas sensor reading
  float analogSensor = analogRead(gas);
  Blynk.virtualWrite(V0, analogSensor);
  Serial.print("Gas Value: ");
  Serial.println(analogSensor);

  
  Serial.print("Latitude:  ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  
  Blynk.virtualWrite(V1, String(latitude, 6));
  Blynk.virtualWrite(V2, String(longitude, 6));
  
  float gasValue=analogRead(gas);
  Serial.println("Gas Value");
  Serial.println(gasValue);
  if(gasValue<400)
  {
     Serial.println("Fresh Air");
    delay(4000);
  }

  else if(gasValue>400)
  {
    Serial.println("Bad Air");  
    delay(4000);
  }

  if(gasValue > 400){
    Blynk.email("YOUR EMAIL", "Alert", "Bad Air,Stay Inside!!");
    Blynk.logEvent("pollution_alert","Bad Air, Stay Inside!! Latitude = " + String(latitude, 6) + "Longitude = " + String(longitude, 6));
  }

}

void setup()
{
 
  Serial.begin(115200);
  Serial2.begin(9600);
  Blynk.begin(auth, ssid, pass);
  delay(10);
  
  timer.setInterval(INTERVAL, sendGps);
 
}


void loop()
{
  Blynk.run();
  timer.run();
}
