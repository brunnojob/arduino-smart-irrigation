#include <Arduino.h>

constexpr uint8_t sensorPin=34, pumpPin=26, buttonPin=27;
constexpr int dryThreshold=2600, wetThreshold=1900;
constexpr unsigned long maxRunMs=12000, lockoutMs=300000;
unsigned long pumpStarted=0, lastStop=0;
bool pumping=false, manual=false;

void stopPump(unsigned long now) {
  digitalWrite(pumpPin,LOW); pumping=false; lastStop=now;
}
void setup() {
  pinMode(pumpPin,OUTPUT); pinMode(buttonPin,INPUT_PULLUP); digitalWrite(pumpPin,LOW);
  Serial.begin(115200);
}
void loop() {
  const unsigned long now=millis();
  const int moisture=analogRead(sensorPin);
  if(digitalRead(buttonPin)==LOW) manual=true;
  if(!pumping && (manual || (moisture>dryThreshold && now-lastStop>lockoutMs))) {
    digitalWrite(pumpPin,HIGH); pumping=true; pumpStarted=now; manual=false;
  }
  if(pumping && (moisture<wetThreshold || now-pumpStarted>=maxRunMs)) stopPump(now);
  Serial.printf("{\"moisture\":%d,\"pump\":%s}\n",moisture,pumping?"true":"false");
  delay(1000);
}