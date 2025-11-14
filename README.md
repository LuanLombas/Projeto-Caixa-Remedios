# Sistema de Irrigação Automatizado com IoT

Este projeto visa otimizar o consumo de água na agricultura (ODS 2 e ODS 6) através de um sistema de irrigação inteligente baseado no ESP8266.

## 📋 Hardware

* **Microcontrolador:** NodeMCU ESP8266
* **Sensor:** Sensor de Umidade do Solo (Módulo YL-69 ou Capacitivo)
* **Atuador:** Módulo Relé 5V + Mini Bomba de Água
* **Conexões:**
    * Sensor (Pino Digital DO) -> NodeMCU D2
    * Relé (Pino IN) -> NodeMCU D1

## 🚀 Execução

1. Carregue o código `codigo.ino` utilizando a Arduino IDE.
2. Certifique-se de ter a biblioteca `PubSubClient` instalada.
3. Configure suas credenciais de Wi-Fi no código.
4. Monitore o tópico MQTT `mackenzie/irrigacao/status`.
