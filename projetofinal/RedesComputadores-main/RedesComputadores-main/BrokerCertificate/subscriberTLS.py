#!/usr/bin/env python
import paho.mqtt.client as mqtt
import ssl, time, inspect, os


broker_address="172.16.158.155" #"test.mosquitto.org"#"RedesSamir"#172.16.158.155" # this must match the CNAME in your server-cert!
broker_port = 8883
topic="T/GettingStarted/pubsub"

def on_message(client, userdata, message):
    print("message received "   , str(message.payload.decode("utf-8")))
    print( "message topic="      , message.topic)
    print( "message qos="        , message.qos)
    print("message retain flag=", message.retain)


print( "creating new instance" )
client = mqtt.Client( "mqttclient" )

print( "connecting to broker" )
client.tls_set(ca_certs = "ca.crt", certfile = "client.crt", keyfile = "client.key", tls_version=ssl.PROTOCOL_TLSv1_2)
client.tls_insecure_set(False)
client.connect( broker_address, broker_port, 60 )

client.loop_start()

print( "Subscribing to topic", topic )
client.on_message=on_message
client.subscribe( topic )

for i in range( 1, 10 ):
    print("Publishing message to topic" , topic )
    client.publish( topic, "Hello world from MQTT "+str(i) )
    time.sleep( 1 )

client.loop_stop()

print( "Goodbye!" )
