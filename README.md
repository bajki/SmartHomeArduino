# SmartHomeArduino
Internet of things this is the Araduno part of the code
This repo is part of my Smart Home project.
This is small REST Servise, installed on Arduni.
It is tried with WeMos D1.
It can be used with Web browser as well as with my Smart Home Android application.

the command url is:
<url_adress>/?command=status

returns JSON with Relay statuses and Temperature and humidity from DHT Sensor

<url_adress>/?command=relayswitch&key=r1&status=1

it switches the relay by that way.
