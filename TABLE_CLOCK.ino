#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

#define STB D2          //lk1668 pins
#define CLK D3
#define DIO D4

unsigned char disp_buffer[16];
int dot=0;

 
const char* ssid = "CAMPUS CONNECT CUP";
const char* password =  "";
 
const char * headerKeys[] = {"date", "server"} ;
const size_t numberOfHeaders = 2;

int hour, minute, second;
unsigned long int mintimer = millis(); 
unsigned long int dottimer = millis();
 
void setup () { 
  pinMode(STB, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  Serial.begin(115200);
  disp_buffer[6] = 0x02;
  disp_buffer[8] = 0x03;
  disp_buffer[10] = 0x02;
  disp_buffer[12] = 0x02;
  display_data();
  WiFi.begin(ssid, password);
  Serial.println("Connecting.. ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  } 
  Serial.println("Connected to WiFi Network");
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http; 
    http.begin("http://jsonplaceholder.typicode.com/posts");  
    http.collectHeaders(headerKeys, numberOfHeaders); 
    int httpCode; 
    do {
      httpCode = http.GET();
      String headerDate = http.header("date");
      Serial.println(headerDate);
      headerDate=headerDate.substring(17);
      Serial.println(headerDate);
      hour = headerDate.toInt() + 5;
      headerDate=headerDate.substring(headerDate.indexOf(":")+1);                     
      minute = (headerDate.toInt() + 38);
      if(minute > 60){
        hour++;
        minute = minute % 60; 
      }
      if(httpCode!=200){
        Serial.println("An error occurred!");
      }
      
    }while(httpCode != 200); 
    http.end(); 
  }
  
}
 
void loop() {
  if(millis() - mintimer >= 60000)
  {
    minute++;
    mintimer = millis();
  }
  if(minute > 59){
    hour++;
    minute = 0;
  }
  if(hour > 12){
    hour = (hour%12==0?12:hour%12);
  }  
  disp_buffer[12]=c2h(minute%10);
  disp_buffer[10]=c2h((minute/10)%10);
  if(millis() - dottimer >=500){
    if(dot == 0){
      dot = 1;
      disp_buffer[8]=c2h(hour%10) + 0x01;
    }
    else{
      dot = 0;
      disp_buffer[8]=c2h(hour%10);
    }
    dottimer = millis();
  }
  //disp_buffer[8]=c2h(hour%10)+0x01;
  disp_buffer[6]=c2h((hour/10)%10);
 
 
  display_data();
  //delay(300);
  
}
