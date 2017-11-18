#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#define DHT22SensorPin D5

ESP8266WebServer server(80);
DHT dht;

const char* ssid = "VIVACOM_IVAN";
const char* password =  "20162016";

int pins[] = { LED_BUILTIN, D7, D8, D9, D0, D1 };   
int pinStatuses[] = {LOW, LOW, LOW, LOW, LOW, LOW};
String pinKeys[] = { "r1","r2","r3","r4","r5","r6"};         
int pinCount = 6;

void handleStatus(){
    String message = "{\"Error\":\"\", ";
    for(int i=0; i<pinCount; i++){
        message += "\"";
        message += pinKeys[i];
        message += "\":";
        message += pinStatuses[i];
        message += ", ";
    }
    
    float h = dht.getHumidity();
    float t = dht.getTemperature();
    message+= "\"t\":";
    message+= t;
    message +=", \"h\":";
    message+= h;

    message += "}\n";
    server.send(200, "application/json", message);
    Serial.println(message);
}

void sendErrorResponce(String error){
      String message = "{\"Error\":\"";
      message += error;
      message += "\"}\n";
      server.send(200, "application/json", message);
      Serial.println(server.arg("command"));
}

void handleSwitch(){
    String key = server.arg("key");
    int relay=-1;
    for(int i=0; i<pinCount; i++){
            if(pinKeys[i].equals(key)){
                relay = i;
                break;
            }
        }
    if(relay == -1){
        sendErrorResponce("WrongSchemaError");
        return;
    }
    int newRelayStatus = server.arg("status").toInt();
    digitalWrite(pins[relay], newRelayStatus==0 ? LOW : HIGH);
    pinStatuses[relay] = newRelayStatus;

    handleStatus();
}

void handleCommand() {
      if (server.hasArg("command")== false){ //Check if body received
            sendErrorResponce("NoCommandError");
            return;
        }
        String command = server.arg("command");
        if(command.equals("switchrelay")){
            handleSwitch();
        } else {
            handleStatus();
        }
}
 
void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);  //Connect to the WiFi network
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
        delay(500);
        Serial.println("Waiting to connect...");
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
    server.on("/", handleCommand); //Associate the handler function to the path
    server.begin(); //Start the server
    Serial.println("Server listening");

    Serial.println("Init Relays");
    for(int i=0; i<pinCount; i++){
        pinMode(pins[i], OUTPUT);
    }
    // for(int i=0; i<pinCount; i++){
        // digitalWrite(pins[i], pinStatuses[i]);
    // }

    Serial.println("Init DHT");
    dht.setup(DHT22SensorPin); // data pin 2
    delay(3000);
}
 
void loop() {
    server.handleClient(); //Handling of incoming requests
}