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

    const char MQTT_HOST[] = "test.mosquitto.org";
    const int MQTT_PORT = 8883;
    const char MQTT_USER[] = ""; // leave blank if no credentials used
    const char MQTT_PASS[] = ""; // leave blank if no credentials used

    const char MQTT_SUB_TOPIC[] = "home/" HOSTNAME "/in";
    const char MQTT_PUB_TOPIC[] = "home/" HOSTNAME "/out";

    #ifdef CHECK_CA_ROOT
    static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEAzCCAuugAwIBAgIUBY1hlCGvdj4NhBXkZ/uLUZNILAwwDQYJKoZIhvcNAQEL
BQAwgZAxCzAJBgNVBAYTAkdCMRcwFQYDVQQIDA5Vbml0ZWQgS2luZ2RvbTEOMAwG
A1UEBwwFRGVyYnkxEjAQBgNVBAoMCU1vc3F1aXR0bzELMAkGA1UECwwCQ0ExFjAU
BgNVBAMMDW1vc3F1aXR0by5vcmcxHzAdBgkqhkiG9w0BCQEWEHJvZ2VyQGF0Y2hv
by5vcmcwHhcNMjAwNjA5MTEwNjM5WhcNMzAwNjA3MTEwNjM5WjCBkDELMAkGA1UE
BhMCR0IxFzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTES
MBAGA1UECgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVp
dHRvLm9yZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzCCASIwDQYJ
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAME0HKmIzfTOwkKLT3THHe+ObdizamPg
UZmD64Tf3zJdNeYGYn4CEXbyP6fy3tWc8S2boW6dzrH8SdFf9uo320GJA9B7U1FW
Te3xda/Lm3JFfaHjkWw7jBwcauQZjpGINHapHRlpiCZsquAthOgxW9SgDgYlGzEA
s06pkEFiMw+qDfLo/sxFKB6vQlFekMeCymjLCbNwPJyqyhFmPWwio/PDMruBTzPH
3cioBnrJWKXc3OjXdLGFJOfj7pP0j/dr2LH72eSvv3PQQFl90CZPFhrCUcRHSSxo
E6yjGOdnz7f6PveLIB574kQORwt8ePn0yidrTC1ictikED3nHYhMUOUCAwEAAaNT
MFEwHQYDVR0OBBYEFPVV6xBUFPiGKDyo5V3+Hbh4N9YSMB8GA1UdIwQYMBaAFPVV
6xBUFPiGKDyo5V3+Hbh4N9YSMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL
BQADggEBAGa9kS21N70ThM6/Hj9D7mbVxKLBjVWe2TPsGfbl3rEDfZ+OKRZ2j6AC
6r7jb4TZO3dzF2p6dgbrlU71Y/4K0TdzIjRj3cQ3KSm41JvUQ0hZ/c04iGDg/xWf
+pp58nfPAYwuerruPNWmlStWAXf0UTqRtg4hQDWBuUFDJTuWuuBvEXudz74eh/wK
sMwfu1HFvjy5Z0iMDU8PUDepjVolOCue9ashlS4EB5IECdSR2TItnAIiIwimx839
LdUdRudafMu5T5Xma182OC0/u/xRlEm+tvKGGmfFcN0piqVl8OrSPBgIlb+1IKJE
m/XriWr/Cq4h/JfB7NTsezVslgkBaoU=
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
    }
}
