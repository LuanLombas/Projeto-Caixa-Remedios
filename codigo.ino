#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// --- Configurações de Hardware ---
// D1: Ligado ao Módulo Relé (que liga a Bomba)
const int PINO_BOMBA = D1; 
// D2: Ligado ao pino Digital (DO) do Sensor de Umidade do Solo
const int PINO_SENSOR = D2; 

// --- Configurações de Rede ---
const char* ssid = "NOME_DA_SUA_REDE";      
const char* password = "SENHA_DA_SUA_REDE"; 

// --- Configurações MQTT ---
const char* mqtt_server = "broker.hivemq.com"; 
const char* topico_status = "mackenzie/irrigacao/status";
const char* topico_comando = "mackenzie/irrigacao/comando";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  pinMode(PINO_BOMBA, OUTPUT);
  pinMode(PINO_SENSOR, INPUT);
  digitalWrite(PINO_BOMBA, HIGH); // Relés geralmente desligam no HIGH

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Aqui entraria a lógica para receber comandos manuais via MQTT
  Serial.print("Mensagem recebida: ");
  Serial.println(topic);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexao MQTT...");
    if (client.connect("ESP8266Irrigacao")) {
      Serial.println("conectado");
      client.subscribe(topico_comando);
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do Sensor (1 = Seco, 0 = Úmido - depende da regulagem do sensor)
  int soloSeco = digitalRead(PINO_SENSOR); 

  if (soloSeco == HIGH) {
    // Solo SECO -> Ligar Bomba
    digitalWrite(PINO_BOMBA, LOW); // Liga o Relé (Low Trigger)
    client.publish(topico_status, "SOLO_SECO_BOMBA_LIGADA");
    Serial.println("Solo Seco - Irrigando...");
  } else {
    // Solo ÚMIDO -> Desligar Bomba
    digitalWrite(PINO_BOMBA, HIGH); // Desliga o Relé
    client.publish(topico_status, "SOLO_UMIDO_BOMBA_DESLIGADA");
    Serial.println("Solo Umido - Monitorando.");
  }
  
  delay(5000); // Espera 5 segundos
}
