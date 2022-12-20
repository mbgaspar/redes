#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
const char* SSID = "abcdefgh"; //Seu SSID da Rede WIFI
const char* PASSWORD = "#######"; // A Senha da Rede WIFI
const char* MQTT_SERVER = "test.mosquitto.org"; //Broker do Mosquitto.org
const char* MQTT_SERVER2 = "maqiatto.com";
int value = 0;
long lastMsg = 0;
char msg[50];
WiFiClient CLIENT;
PubSubClient MQTT(CLIENT);
//CONFIGURAÇÃO DA INTERFACE DE REDE
void setupWIFI() {
WiFi.begin(SSID, PASSWORD);
Serial.print("Conectando na rede: ");
Serial.println(SSID);
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
}
void setup(void) {
Serial.begin(115200);
setupWIFI();
MQTT.setServer(MQTT_SERVER, 1883);
}
void reconectar() {
while (!MQTT.connected()) {
Serial.println("Conectando ao Broker MQTT.");
if (MQTT.connect("ESP8266")) {
Serial.println("Conectado com Sucesso ao Broker");
} else {
Serial.print("Falha ao Conectador, rc=");
Serial.print(MQTT.state());
Serial.println(" ...tentando se reconectar...");
delay(3000);
}
}
}
void loop(void) {
if (!MQTT.connected()) {
reconectar();
}
MQTT.loop();
long now = millis();
if (now - lastMsg > 2000) {
lastMsg = now;
value++;
if(value >=50) value = -20;
snprintf (msg, 75, "%ld", value);
Serial.print("Uando topico temp/random. Mensagem a ser publicada: ");
Serial.println(msg);
MQTT.publish("temp/random", msg);
}
}
