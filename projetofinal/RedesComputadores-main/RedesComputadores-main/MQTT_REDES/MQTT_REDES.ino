#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

// CONFIGURAÇÃO DA INTERFACE DE REDE
const char* ssid = "redes_mqtt";     // Seu SSID da Rede WIFI
const char* PASSWORD = "redes22108"; // A Senha da Rede WIFI
// Server
const char* MQTT_SERVER = "172.16.158.155";
#define PORT 1883
#define rele 16
WiFiClient CLIENT;
PubSubClient client(CLIENT);

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;
const int ledPin = 4;

String msg_String;


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
  if (String(topic) == "esp8266/teste")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "HIGH")
    {
      Serial.println("off");
      digitalWrite(rele, HIGH);
      messageTemp.toCharArray(msg, 50);
    }
    else if (messageTemp == "LOW")
    {
      Serial.println("on");
      digitalWrite(rele, LOW);
      messageTemp.toCharArray(msg, 50);
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
    if (client.connect("ESP8266Client2"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp8266/teste");
      // Testing
      client.publish("esp8266/teste", msg);
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
  Serial.print("Esse é o port: ");
  Serial.print(PORT);
  // set callback when receive msgs
  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT); //Define led mode
  pinMode(rele, OUTPUT); // Define o pino do rele e seu estado inicial
}


void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if(WiFi.status() != WL_CONNECTED){
    setupWIFI();
  }
  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;

    Serial.print("Enviado para os topico esp8266/teste: \n");
    Serial.println(msg);
    Serial.print("\n");
    client.publish("esp8266/teste", msg);
  }
}
