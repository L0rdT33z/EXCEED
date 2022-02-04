#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>

//const char* ssid="HARDWARE_hostpot";
//const char* password = "1Q2w3e4%";
const char* ssid="Tee";
const char* password = "11111111";
const char* url = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=13";

int ldr=34;
int led1=5;
int led2=22;

static int ldr_val;
static int led1_val;
static int led2_val;
char str[50];


const int _size = 2*JSON_OBJECT_SIZE(3);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;

void WiFi_Connect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void _post(int ldr_post, int led1_post, int led2_post){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(url);
    http.addHeader("Content-Type","application/json");
  
    JSONPost["ldr"] = ldr_post;
    JSONPost["led1"] = led1_post;
    JSONPost["led2"] = led2_post;
    serializeJson(JSONPost,str);
    int httpCode = http.POST(str);

    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      
    }else{
      Serial.println(httoCode);
      Serial.println("ERROR on HTTP Request");
    }
  }
  
}

void setup(){
  pinMode(ldr,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);

  digitalWrite(led1,HIGH);
  digitalWrite(led2,LOW);
  Serial.begin(115200);
  delay(4000);
  WiFi_Connect();
}

void loop(){
  _post(analogRead(ldr), !digitalRead(led1), !digitalRead(led2));
  delay(1000);
}
   
