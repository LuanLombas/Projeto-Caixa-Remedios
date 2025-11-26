#include <WiFi.h>
#include <PubSubClient.h>

// --- PINOS ESP32 (ODS 3 - Saúde) ---
const int PINO_ALARME = 15; // LED (Simula Alerta)
const int PINO_SENSOR = 4;  // Chave (Simula Tampa)

// --- REDE ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(PINO_ALARME, OUTPUT);
  pinMode(PINO_SENSOR, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32-Saude-Final")) {
      client.subscribe("mackenzie/saude/comando");
    } else delay(5000);
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Se chave ligada (GND) = Caixa Fechada. Se solta = Aberta.
  int tampa = digitalRead(PINO_SENSOR);

  if (tampa == HIGH) { 
    digitalWrite(PINO_ALARME, LOW); 
    client.publish("mackenzie/saude/status", "REMEDIO_TOMADO_CAIXA_ABERTA");
  } else { 
    digitalWrite(PINO_ALARME, HIGH); 
    client.publish("mackenzie/saude/status", "ALERTA_HORA_DO_REMEDIO");
  }
  delay(2000);
}
