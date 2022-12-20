#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

// CONFIGURAÇÃO DA INTERFACE DE REDE
const char *SSID = "redes_mqtt";     // Seu SSID da Rede WIFI
const char *PASSWORD = "redes22108"; // A Senha da Rede WIFI
// Server
const char *MQTT_SERVER = "172.16.159.222";
#define PORT 1883

WiFiClient CLIENT;
PubSubClient MQTT(CLIENT);

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature =0;
float humidity = 0;
const int ledPin = 4;


void setup()
{
    // Start serial - Serial monitor
    Serial.begin(115200);
    // Config Wifi - Get access to wifi
    setupWIFI();
    // set server to access mqtt
    MQTT.setServer(MQTT_SERVER, PORT);

    // set callback when receive msgs
    MQTT.setCallback(callback);

    pinMode(ledPin, OUTPUT); //Define led mode
}

// WIFI CONFIGURATION
void setupWIFI()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PASSWORD);

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
            digitalWrite(ledPin, HIGH);
        }
        else if (messageTemp == "off")
        {
            Serial.println("off");
            digitalWrite(ledPin, LOW);
        }
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!MQTT.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (MQTT.connect("ESP8266"))
        {
            Serial.println("connected");
            // Subscribe
            MQTT.subscribe("samir.bonho@ifsc.edu.br/temperatura");
            MQTT.subscribe("samir.bonho@ifsc.edu.br/altura");
            MQTT.subscribe("samir.bonho@ifsc.edu.br/luminosidade");
            MQTT.subscribe("samir.bonho@ifsc.edu.br/umidade");
            // Testing
            MQTT.publish("samir.bonho@ifsc.edu.br/temperatura", "25");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(MQTT.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}




void loop()
{
    if (!MQTT.connected())
    {
        reconnect();
    }
    MQTT.loop();

    long now = millis();
    if (now - lastMsg > 5000)
    {
        lastMsg = now;
        value++;
        if (value >= 50) value = -20;
        snprintf (msg, 75, "%ld", value);
    
        Serial.print("Enviado para os topicos temperatura, altura, luminosidade, umidade o valor :");
        Serial.println(msg);

        MQTT.publish("samir.bonho@ifsc.edu.br/temperatura",   msg);
        MQTT.publish("samir.bonho@ifsc.edu.br/altura",        msg);
        MQTT.publish("samir.bonho@ifsc.edu.br/luminosidade",  msg);
        MQTT.publish("samir.bonho@ifsc.edu.br/umidade",       msg);

    }
}
