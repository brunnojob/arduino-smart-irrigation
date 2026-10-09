#include "irrigation.hpp"
#include <Arduino.h>
constexpr int sensorPin = 34, pumpPin = 26, buttonPin = 27, reservoirPin = 25;
IrrigationController controller;
std::uint32_t lastSample = 0, buttonChanged = 0;
bool previousButton = false, stableButton = false;
void setup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(reservoirPin, INPUT_PULLUP);
  analogReadResolution(12);
  Serial.begin(115200);
}
void loop() {
  std::uint32_t now = millis();
  digitalWrite(pumpPin, controller.tick(now).pump ? HIGH : LOW);
  bool button = digitalRead(buttonPin) == LOW;
  if (button != previousButton) {
    previousButton = button;
    buttonChanged = now;
  }
  bool edge = false;
  if (std::uint32_t(now - buttonChanged) >= 50 && button != stableButton) {
    stableButton = button;
    edge = button;
  }
  if (std::uint32_t(now - lastSample) >= 1000 || edge) {
    lastSample = now;
    auto status = controller.sample(
        analogRead(sensorPin), digitalRead(reservoirPin) == LOW, edge, now);
    digitalWrite(pumpPin, status.pump ? HIGH : LOW);
    Serial.printf(
        "{\"deviceId\":\"irrigation-01\",\"sequence\":%lu,\"timestampMs\":%lu,"
        "\"moisture\":%d,\"pump\":%s,\"state\":%d,\"reason\":\"%s\"}\n",
        static_cast<unsigned long>(status.sequence),
        static_cast<unsigned long>(now), status.moisture,
        status.pump ? "true" : "false", int(status.state), status.reason);
  }
}
