#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

// CONFIGURAÇÃO DA INTERFACE DE REDE
const char* ssid = "redes_mqtt";     // Seu SSID da Rede WIFI
const char* PASSWORD = "redes22108"; // A Senha da Rede WIFI
// Server
const char* MQTT_SERVER = "172.16.158.155";
#define PORT 1883
#define RELE D2 // Pino I/O
WiFiClient CLIENT;
PubSubClient client(CLIENT);

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;
const int ledPin = 4;

bool rele = false; // Marca o estado inicial do relé "desligado"

// WIFI CONFIGURATION
void setupWIFI()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT STUFFS
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic esp32/output,
  // you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "esp32/output")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "on")
    {
      Serial.println("on");
      if (rele == false) {
        digitalWrite(RELE, HIGH);
        rele = true;
      }
      messageTemp.toCharArray("HIGH", 50);
    }
    else if (messageTemp == "off")
    {
      Serial.println("off");
      if (rele == true) {
        digitalWrite(RELE, LOW);
        rele = false;
      }
      messageTemp.toCharArray("LOW", 50);
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client4"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp8266/teste");
      //client.subscribe("inTopic");
      // Testing
      client.publish("esp8266/teste", msg);
      //client.publish("teste/oi", "hello world");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

}


void setup()
{
  // Start serial - Serial monitor
  Serial.begin(115200);
  // Config Wifi - Get access to wifi
  setupWIFI();
  // set server to access mqtt
  client.setServer(MQTT_SERVER, PORT);
  Serial.print("Essa é a porta: ");
  Serial.print(PORT);
  Serial.print("\n");
  // set callback when receive msgs
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT); //Define led mode
  pinMode(RELE, OUTPUT); // Define o pino do rele
  digitalWrite(RELE, LOW); // inicialmente deixa o relé desligado
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (WiFi.status() != WL_CONNECTED) {
    setupWIFI();
  }
  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;
    value++;
    if (value >= 50) value = -20;
    snprintf (msg, 75, "%ld", value);

    Serial.print("Enviado para os topicos temperatura, altura, luminosidade, umidade o valor :");
    Serial.println(msg);
    Serial.print("\n");
    client.publish("esp8266/teste", msg);

    /* client.publish("samir.bonho@ifsc.edu.br/temperatura",   msg);
      client.publish("samir.bonho@ifsc.edu.br/altura",        msg);
      client.publish("samir.bonho@ifsc.edu.br/luminosidade",  msg);
      client.publish("samir.bonho@ifsc.edu.br/umidade",       msg);*/
  }
  tarefa_rele();
}


void tarefa_rele() {

  /* Caso tenha recebido algum dado do PC */
  if (Serial.available()) {
    char dado_recebido = Serial.read();

    /* Depuração */
    Serial.print("Recebido:");
    Serial.println(dado_recebido);

    if (dado_recebido == 'r') {// abre ou fecha o relé
      if (rele == true) {
        rele = false;
        digitalWrite(RELE, LOW);
      }
      else {
        rele = true;
        digitalWrite(RELE, HIGH);
      }
    }
  }
}
