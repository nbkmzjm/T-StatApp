/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 9

 created 25 Nov 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

char ssid[] = "ThienIphone";      //  your network SSID (name)
char pass[] = "aivynguyen";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
const int acPin = 4;
#define DHTPin 5
#define HEATPin 13
#define ACPin 15
#define DHTTYPE DHT22

DHT dht(DHTPin, DHTTYPE);
float hum;
float temp;
float sTemp = 78;
String modeStatus = "";
String c;
int i = 1;
int status = WL_IDLE_STATUS;

char host[] = "tstat.herokuapp.com"; 
WiFiClient client;

long lastConTime = 0;
const long delayInterval= 2L * 1000L;

void setup() {
  Serial.begin(115200);      // initialize serial communication
  dht.begin();
  pinMode(ACPin, OUTPUT);      // set the LED pin mode
  pinMode(HEATPin, OUTPUT);      // set the LED pin mode
  
  

 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(6000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();                        // you're connected now, so print out the status

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(host, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
//    client.println("GET /tstat?temp=90 HTTP/1.1");
//    client.println("Host: tstat.herokuapp.com");
//    client.println("Connection: close");
//    client.println();
  }

  digitalWrite(ACPin, LOW);
  digitalWrite(HEATPin, LOW);
 
}


void loop() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
//  Serial.println("Humidity: \n");
//  Serial.println(hum);
//  Serial.println("\nTemp: \n");
//  Serial.println(temp);
  // arrMode [] = [ACPin,HEATPin]
  // for(i=0; i<sizeof(arrMode)-1; i++){}
  if(ACPin=='HIGH'){
    modeStatus= "AC";
  }else if(HEATPin=='HIGH'){
    modeStatus = "HEAT";
  }else{
    modeStatus = "OFF";
  }
  
  if (millis()- lastConTime > delayInterval){
    client.stop();
    if(client.connect(host, 80)){
//      client.connect(host, 80);
//      
//      String url = "/tstatMoni";
//      url += "?temp=";
//      url += temp;
//      url += "&humid=";
//      url += hum;
//      url += "&mode=";
//      url += modeStatus;
//      url += "&sTemp=";
//      url += sTemp;
//      
//      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                   "Host: " + host + "\r\n" + 
//                   "Connection: close\r\n\r\n");
      
      client.connect(host, 80);
      String urlparam = "";
      urlparam += "mode=";
      urlparam += temp;
      urlparam += "&sTemp=";
      urlparam += hum;
      Serial.println(urlparam.length());
      client.print(String("POST /tstatMode") + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n" + 
                   "Content-Length: " + urlparam.length()+ "\r\n\r\n"+
                   urlparam+ "\r\n");


                   
      lastConTime = millis();
    }else{
      Serial.println("connection failed");
    }
  }
  

  
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    c = client.readStringUntil('\r');
    if(i==10){
//      if(c=="xx"){
//        digitalWrite(HEATPin, HIGH);
      
     Serial.print(c);
     
     
    i = 0; 
    }
     
    
    i++;
  }
  
  
  // if the server's disconnected, stop the client:
//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("disconnecting from server.");
//    client.stop();
//
//    // do nothing forevermore:
//    while (true);
//  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
