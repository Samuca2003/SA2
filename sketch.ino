#include <WiFi.h>
#include <HTTPClient.h>
#include "DHTesp.h"
#include "ThingSpeak.h" 
#define  DHT 12

DHTesp sensor;

const char* ssid = "Wokwi-GUEST";
const char* password =  "";
// Write a channel field
const char* url = "https://api.thingspeak.com/update?api_key=H6N85STDEFGT56GB&field1=0&field2=0"; 
HTTPClient http;
WiFiClient client;
unsigned long ChannelNumber = 2101238; // Channel Id
const char * APIKey = "H6N85STDEFGT56GB"; // Write API Key


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S2!");
  pinMode(19, OUTPUT);
  pinMode(18, INPUT);

  sensor.setup(DHT, DHTesp::DHT22);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(5000);
    Serial.println("Conectando no Wifi...");
  }

  Serial.println("Conectado");
  ThingSpeak.begin(client); 
}

float getTemperatura(){
  return sensor.getTemperature();
}
float getHumidade(){
  return sensor.getHumidity();
}

void loop() {
  delay(5000);
  float temperatura = getTemperatura();
  float humidade = getHumidade();
  Serial.println("Temperatura: " + String(temperatura) + " °C " );
  delay(5000);
  Serial.println("Umidade: " + String(humidade) + " % ");
  delay(5000);
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, humidade);
  
  if (temperatura > 40) {
    Serial.print("ATENÇÂO!! Temperatura maior que 40°, desligue o equipamento!");
    digitalWrite(19, HIGH);
    delay(temperatura>40);
  }else{
    digitalWrite(19, LOW);
    delay(5000);
  }

  if (humidade > 80) {
    Serial.print("ATENÇÂO!! Humidade maior que 80°, desligue o equipamento!");
    digitalWrite(18, HIGH);
    delay(humidade>40);
  }else{
    digitalWrite(18, LOW);
    //delay(5000);
  }

 // put your main code here, to run repeatedly:
  http.begin(url);
  int httpCode = http.GET();
  Serial.println(httpCode);
  
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Erro ao requisitar URL");
  }
  
  int x = ThingSpeak.writeFields(ChannelNumber, APIKey);
  
  if(x == 200){
    Serial.println("Atualização do canal com sucesso.");
  } else {
    Serial.println("Problema ao atualizar o canal. Código de erro HTTP " + String(x));
  }
  
  
  // delay(5000); // this speeds up the simulation
}
