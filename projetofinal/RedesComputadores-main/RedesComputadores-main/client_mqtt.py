
import paho.mqtt.client as mqtt
import random
import time

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("esp8266/teste")
    client.publish("esp8266/teste","teste")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    estados = ['LOW', 'HIGH']
    lastState = random.randint(0, 1)
    client.publish("esp8266/teste", estados[lastState]) 
    print(msg.topic+" "+str(msg.payload))
    time.sleep(2)
    

def main():

    print('Configuring client')
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    print('Trying to connect to server')
    client.connect("172.16.158.155", 1883, 60)

    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    # Other loop*() functions are available that give a threaded interface and a
    # manual interface.
    client.loop_forever()

if __name__ == '__main__':
    print('program started')
    main()
