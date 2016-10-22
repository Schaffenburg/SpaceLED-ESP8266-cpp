
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
// WiFi information
const char WIFI_SSID[] = "KellerkindGast";
const char WIFI_PSK[] = "1234567890";
 
// Remote site information
const char* http_site = "status.schaffenburg.org";
const int http_port = 80;

// Global variables
WiFiClient client;
 
void setup() {
  
  // Set up serial console to read web page
  Serial.begin(115200);
  
  // Wait for Serial Port to be ready
  while (!Serial) {
    delay(1);
  }
 
  // Connect to WiFi
  connectWiFi();
  
    
}
 
void loop() {
  // Attempt to connect to website 
  if ( !getPage() ) {
    Serial.println("GET request failed");
  }
  
  while(!client.available()) {
    delay(1);
  }
  // If there are incoming bytes, print them
  if( client.available() ) {

   
   while(client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    delay(2000);
   }
  
  
  }
  client.flush();

  
}
 
// Attempt to connect to Wifi
void connectWiFi() {

  Serial.println();
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  
}
 
// Perform an HTTP GET request to a remote page
bool getPage() {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.println("GET /index.html HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}
