#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN 15
#define PINwhite 5
#define PINblue 4
#define PINred 2
#define PINgreen 0

const char* ssid     = "KellerkindGast";  
const char* password = "1234567890";
const char* led      = "normal"; // "intelligent" or "normal" led output

const char* host     = "status.schaffenburg.org"; // Your domain  
String path          = "/index.html";  
const int pin        = 2;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60,PIN,NEO_RGB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();

  pinMode(PINwhite,OUTPUT);
  pinMode(PINblue,OUTPUT);
  pinMode(PINred,OUTPUT);
  pinMode(PINgreen,OUTPUT);
  digitalWrite(PINwhite,LOW);
  digitalWrite(PINblue,LOW);
  digitalWrite(PINred,LOW);
  digitalWrite(PINgreen,LOW);
    
  if(led == "normal") {
    digitalWrite(PINwhite,HIGH);
  }

  if(led == "intelligent") {
    strip.begin();
    strip.setPixelColor(0,120,120,120);
    strip.show();
  }
  
  Serial.begin(115200);

  delay(10);
  
}

void loop() { 
  
  WiFi.begin(ssid, password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
   
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500);

  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    
    if (section=="header") { 
      if (line=="\n") {  
        section="status";
      }
    }
    else if (section=="status") {
      if(line.substring(1,2) == "v") {
        Serial.println("zu");
        if(led == "intelligent") {
          strip.setPixelColor(0,255,0,0);
          strip.show();
        }
        
        if(led == "normal") {
          digitalWrite(PINwhite,LOW);
          digitalWrite(PINblue,LOW);
          digitalWrite(PINred,HIGH);
          digitalWrite(PINgreen,LOW);
        }      
      } else if (line.substring(1,2) == "o") {
        Serial.println("offen");
        if(led == "intelligent") {
          strip.setPixelColor(0,0,255,0);
          strip.show();
        }

        if(led == "normal") {
          digitalWrite(PINwhite,LOW);
          digitalWrite(PINblue,LOW);
          digitalWrite(PINred,LOW);
          digitalWrite(PINgreen,HIGH);
        }
      } else {
        Serial.println("gestört");
        if(led == "intelligent") {
          strip.setPixelColor(0,0,0,255);
          strip.show();
        }

        if(led == "normal") {
          digitalWrite(PINwhite,LOW);
          digitalWrite(PINblue,HIGH);
          digitalWrite(PINred,LOW);
          digitalWrite(PINgreen,LOW);
        }
      }
    }
  }

  WiFi.disconnect();
  
  delay(30000);
}
  
