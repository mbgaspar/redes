#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <time.h>

//enable only one of these below, disabling both is fine too.
#define CHECK_CA_ROOT
//#define CHECK_PUB_KEY
//#define CHECK_FINGERPRINT
////--------------------------////

#ifndef SECRET
    const char ssid[] = "redes_mqtt";
    const char pass[] = "redes22108";

    #define HOSTNAME "esp8266_mqtt_client1"

    const char MQTT_HOST[] = "172.16.158.155";
    const int MQTT_PORT = 8883;
    const char MQTT_USER[] = ""; // leave blank if no credentials used
    const char MQTT_PASS[] = ""; // leave blank if no credentials used

    const char MQTT_SUB_TOPIC[] = "home/" HOSTNAME "/in";
    const char MQTT_PUB_TOPIC[] = "home/" HOSTNAME "/out";
    const char MQTT_SUB_TOPIC2[] = "batatinha";
    const char MQTT_PUB_TOPIC2[] = "fofuxo";

    #ifdef CHECK_CA_ROOT
    static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDzTCCArWgAwIBAgIUbGwEwlE705f2phrYXOm7KNKuYywwDQYJKoZIhvcNAQEL
BQAwdjELMAkGA1UEBhMCQlIxFzAVBgNVBAgMDlNhbnRhLUNhdGFyaW5hMRYwFAYD
VQQHDA1GbG9yaWFub3BvbGlzMQ0wCwYDVQQKDARJRlNDMQ4wDAYDVQQLDAVSZWRl
czEXMBUGA1UEAwwOMTcyLjE2LjE1OC4xNTUwHhcNMjIwNjA3MTM0MjUxWhcNMjMw
NjA3MTM0MjUxWjB2MQswCQYDVQQGEwJCUjEXMBUGA1UECAwOU2FudGEtQ2F0YXJp
bmExFjAUBgNVBAcMDUZsb3JpYW5vcG9saXMxDTALBgNVBAoMBElGU0MxDjAMBgNV
BAsMBVJlZGVzMRcwFQYDVQQDDA4xNzIuMTYuMTU4LjE1NTCCASIwDQYJKoZIhvcN
AQEBBQADggEPADCCAQoCggEBALL2AkeRHX6CCe0F3Y/CdkVZ5EBiXG55mpCuUhPi
AnGrf/xPVn5KBmZTTHJ2qk3M4z+STcuAfJ7SKU2fISoYMW/Wt0KI6nhr9BFa64/f
UBCMfVM8uqkRF5fmFI3zhNef0Vz+8GeXZK0uRjsP32lNEuU/lUB7hUpfSg4IyRYS
u8Oe10vMykGxXAG/YsTnaNCwhyNJYT1n/I1BXhB64Ectf72BdHWshEFA0ghgJE+c
Q7dmDm2C6xQq+sAYfFPmQGCYbaa3oac9UpTiJbosNlgBkE9agtDYIPQ4sKC8PWOu
1GGH20VnkjP/2GXpyfHH5T5mK8/+pOYmHgCC8hnf/4gDJSUCAwEAAaNTMFEwHQYD
VR0OBBYEFOv0/rvw/r+AOG/X19+wwgT66E0FMB8GA1UdIwQYMBaAFOv0/rvw/r+A
OG/X19+wwgT66E0FMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEB
ACZRfiS5LdPxTbTj+95Y+iTPyxCktkGCeQ3dFqwmhLasrHxXgXIxfgi9tdKdUz7q
AuKv7QB2PEjvxDK+tn7UgyYaP9ODV0v7cEACyg1Xz93TSAgGtON0ZNyHU5WK4X05
yNdjmVDosM4xnMy78oQfUNKaQcusKYbiN+fpBhNtKnCR3BsY+B/GBSlNNQmasbt/
IN8zF77ckd8DPgFihhXJvr3qG8BPX7wp08zOLjW4wBiU6eOZMUlUyKV7yOsjr3Vd
/iPLzfRC3422Ti90qCtD6yPo8hY7kG1gqPNoK/pEN6e8Xxg2CzCjn3tDd24Ya9Yn
xzdZG+GbooIlOD0w8UPynYc=
-----END CERTIFICATE-----
    )EOF";
    #endif

    #ifdef CHECK_PUB_KEY
    // Extracted by: openssl x509 -pubkey -noout -in ca.crt
    static const char pubkey[] PROGMEM = R"KEY(
    -----BEGIN PUBLIC KEY-----
30 82 01 0a 02 82 01 01 00 c1 34 1c a9 88 cd f4 ce c2 42 8b 
4f 74 c7 1d ef 8e 6d d8 b3 6a 63 e0 51 99 83 eb 84 df df 32 
5d 35 e6 06 62 7e 02 11 76 f2 3f a7 f2 de d5 9c f1 2d 9b a1 
6e 9d ce b1 fc 49 d1 5f f6 ea 37 db 41 89 03 d0 7b 53 51 56 
4d ed f1 75 af cb 9b 72 45 7d a1 e3 91 6c 3b 8c 1c 1c 6a e4 
19 8e 91 88 34 76 a9 1d 19 69 88 26 6c aa e0 2d 84 e8 31 5b 
d4 a0 0e 06 25 1b 31 00 b3 4e a9 90 41 62 33 0f aa 0d f2 e8 
fe cc 45 28 1e af 42 51 5e 90 c7 82 ca 68 cb 09 b3 70 3c 9c 
aa ca 11 66 3d 6c 22 a3 f3 c3 32 bb 81 4f 33 c7 dd c8 a8 06 
7a c9 58 a5 dc dc e8 d7 74 b1 85 24 e7 e3 ee 93 f4 8f f7 6b 
d8 b1 fb d9 e4 af bf 73 d0 40 59 7d d0 26 4f 16 1a c2 51 c4 
47 49 2c 68 13 ac a3 18 e7 67 cf b7 fa 3e f7 8b 20 1e 7b e2 
44 0e 47 0b 7c 78 f9 f4 ca 27 6b 4c 2d 62 72 d8 a4 10 3d e7 
1d 88 4c 50 e5 02 03 01 00 01
    -----END PUBLIC KEY-----
    )KEY";
    #endif

    #ifdef CHECK_FINGERPRINT
  // Extracted by: openssl x509 -fingerprint -in ca.crt
    static const char fp[] PROGMEM = "06:90:7E:A4:Ec:A0:F7:70:28:12:DC:51:1F:ED:62:A7:99:AC:71:9A";
    #endif
#endif
//////////////////////////////////////////////////////

#if (defined(CHECK_PUB_KEY) and defined(CHECK_CA_ROOT)) or (defined(CHECK_PUB_KEY) and defined(CHECK_FINGERPRINT)) or (defined(CHECK_FINGERPRINT) and defined(CHECK_CA_ROOT)) or (defined(CHECK_PUB_KEY) and defined(CHECK_CA_ROOT) and defined(CHECK_FINGERPRINT))
#error "cant have both CHECK_CA_ROOT and CHECK_PUB_KEY enabled"
#endif

BearSSL::WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;
time_t now;

void mqtt_connect()
{
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("\nMQTT connecting ");
    while (!client.connect(HOSTNAME))
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("connected!");

    client.subscribe(MQTT_SUB_TOPIC);
    client.subscribe(MQTT_SUB_TOPIC2);
}

void messageReceived(String &topic, String &payload)
{
    Serial.println("Recieved [" + topic + "]: " + payload);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.print("Attempting to connect to SSID: ");
    Serial.print(ssid);
    WiFi.hostname(HOSTNAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("connected!");
    Serial.println(WiFi.localIP());
    Serial.print("Setting time using SNTP");
    configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    while (now < 1510592825)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("done!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));

    #ifdef CHECK_CA_ROOT
        BearSSL::X509List cert(digicert);
        net.setTrustAnchors(&cert);
    #endif
    #ifdef CHECK_PUB_KEY
        BearSSL::PublicKey key(pubkey);
        net.setKnownKey(&key);
    #endif
    #ifdef CHECK_FINGERPRINT
        net.setFingerprint(fp);
    #endif
    #if (!defined(CHECK_PUB_KEY) and !defined(CHECK_CA_ROOT) and !defined(CHECK_FINGERPRINT))
        net.setInsecure();
    #endif

    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.onMessage(messageReceived);

    mqtt_connect();
}

void loop()
{
    now = time(nullptr);
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Checking wifi");
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            WiFi.begin(ssid, pass);
            Serial.print(".");
            delay(10);
        }
        Serial.println("connected");
    }
    else
    {
        if (!client.connected())
        {
            mqtt_connect();
        }
        else
        {
            client.loop();
        }
    }

    // publish a message roughly every second.
    if (millis() - lastMillis > 5000)
    {
        lastMillis = millis();
        client.publish(MQTT_PUB_TOPIC, ctime(&now), false, 0);
        client.publish(MQTT_PUB_TOPIC2, "teste mqtt", false, 0);
    }
}
