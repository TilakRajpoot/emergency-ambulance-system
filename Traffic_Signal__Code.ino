#include <WiFi.h>
#include <esp_now.h>

// ✅ Updated pin numbers
#define RED_PIN 4
#define YELLOW_PIN 5
#define GREEN_PIN 2

typedef struct struct_message {
  char message[32];
} struct_message;

struct_message incomingMessage;
bool ambulanceNearby = false;
unsigned long lastAmbulanceTime = 0;

void turnRed() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void turnYellow() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
}

void turnGreen() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
}

// ✅ Callback function for ESP32 v3.x.x
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  if (strcmp(incomingMessage.message, "AMBULANCE") == 0) {
    Serial.println("🚑 Ambulance Detected! Turning Green...");
    ambulanceNearby = true;
    lastAmbulanceTime = millis();
    turnGreen();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  turnRed();  // Default traffic light is Red

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  if (ambulanceNearby && millis() - lastAmbulanceTime > 5000) {
    Serial.println("🚦 Restoring Normal Traffic...");
    turnRed();
    ambulanceNearby = false;
  }
}
