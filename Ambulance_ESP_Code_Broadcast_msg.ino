#include <WiFi.h>
#include <esp_now.h>

// Broadcast to all ESPs
uint8_t broadcastAddress[] = {0x3c, 0x8a, 0x1f, 0xa9, 0x64, 0x8c};  

typedef struct struct_message {
  char message[32];
} struct_message;

struct_message outgoingMessage;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW Init Failed!");
    return;
  }

  // Create peer info structure
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
    return;
  }
}

void loop() {
  Serial.println("📡 Broadcasting: AMBULANCE");
  strcpy(outgoingMessage.message, "AMBULANCE");

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));

  if (result == ESP_OK) {
    Serial.println("✅ Message sent successfully");
  } else {
    Serial.println("❌ Error sending the message");
  }

  delay(1000); // Broadcast every second
}
