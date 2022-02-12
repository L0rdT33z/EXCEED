# HTTP Request

Library : 

WiFi.h - เชื่อม Wifi

HTTPClient.h - ส่ง HTTP Request ได้

ArduinoJson.h - ส่ง json ได้

## WiFi.h

WiFi.begin(ssid, password) - เชื่อมต่อ WiFi

WiFi.status() - เช็คสถานะ 

- WL_NO_SHIELD = 255

- WL_IDLE_STATUS = 0

- WL_NO_SSID_AVAIL = 1

- WL_SCAN_COMPLETED = 2

- WL_CONNECTED = 3

- WL_CONNECT_FAILED = 4

- WL_CONNECTION_LOST = 5

- WL_DISCONNECTED = 6

WiFi.localIP() - IP ในวง

WiFi.disconnect()

WiFI.reconnect()

- แนะนำให้ใช้ diconnect แล้ว begin ดีกว่า

ใช้ Hostpot หรือ WiFi บ้านได้ แต่ไม่แนะนำ KUWIN เพราะติด auth



## HTTPClient.h

HTTPClient

HTTPClient::begin(URL) - เชื่อมกับ url ที่กำหนด

HTTPClient::GET() - ส่ง GET -> return เป็น status ส่วนข้อมูลเก็บใน payload

HTTPClient::getString() - แสดงผลจาก payload return เป็นก้อนข้อมูลในรูป String

HTTPClient::POST(payload) - ส่ง POST ไปที่ URL

HTTPClient::addHeader("Content-Type", Type) - กำหนด Header



### Exercise Simple GET

```

#include<WiFi.h>
#include<HTTPClient.h>

const char* ssid="HARDWARE_hostpot";
const char* password = "1Q2w3e4%";
const char* url = "*REDACTED FOR PRIVACY*";

void setup(){
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi network");
}

void loop(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if(httpCode<400){
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }else{
      Serial.println("ERROR on HTTP Request");
    }
  }
  delay(10000);
}


```

## ArduinoJson.h

StaticJsonDocument<size> JSON - Fix ขนาด

DynamicJsonDocument JSON(size) - ตัวแปรกำหนดขนาด

JSON_OBJECT_SIZE(no_of_member)

serializeJson(JSON, string)

DeserializationError err = deserializeJson(JSON, string) - ตรวจ Error

### Exercise : Simple POST

```
#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>

const char* ssid="HARDWARE_hostpot";
const char* password = "1Q2w3e4%";
const char* url = "*REDACTED FOR PRIVACY*";

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
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  }else{
    WiFi_Connect();
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
   
```

### Exercise : GET + POST

```
#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>

h>const char* ssid="HARDWARE_hostpot";
const char* password = "1Q2w3e4%";
const char* url = "*REDACTED FOR PRIVACY*";

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

void _get(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(url);

    int httpCode = http.GET();

    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      DeserializationError err = deserializeJson(JSONGet, payload);
      if(err){
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
      }else{
        Serial.println(httpCode);
        Serial.print("ldr : ");
        Serial.println((int)JSONGet["ldr"]);
        Serial.print("led1 : ");
        Serial.println((int)JSONGet["led1"]);
        Serial.print("led2 : ");
        Serial.println((int)JSONGet["led2"]);
      }
    }else{
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  }else{
    WiFi_Connect();
  }
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
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  }else{
    WiFi_Connect();
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
  _get();
  digitalWrite(led1,(int)JSONGet["led1"]);
  digitalWrite(led2,(int)JSONGet["led2"]);
  delay(1000);
}

```

### Exercise : RTOS GET POST

```
#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>

#define tckMs portTICK_PERIOD_MS

const char* ssid="HARDWARE_hostpot";
const char* password = "1Q2w3e4%";
const char* url = "*REDACTED FOR PRIVACY*";

int ldr=34;
int led1=5;
int led2=22;

static int ldrVal = 0;
static int led1Val = 0;
static int led2Val = 0;
char str[50];


const int _size = 2*JSON_OBJECT_SIZE(3);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;

static TaskHandle_t taskGet = NULL;
static TaskHandle_t taskPost = NULL;

void assignParam(){
  ldrVal = analogRead(ldr);
  led1Val = digitalRead(led1);
  led2Val = digitalRead(led2);
}

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

void _get(void *param){
  while(1){
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
  
      http.begin(url);
  
      int httpCode = http.GET();
  
      if(httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        DeserializationError err = deserializeJson(JSONGet, payload);
        if(err){
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(err.c_str());
        }else{
          Serial.println(httpCode);
          Serial.print("ldr : ");
          Serial.println((int)JSONGet["ldr"]);
          Serial.print("led1 : ");
          Serial.println((int)JSONGet["led1"]);
          Serial.print("led2 : ");
          Serial.println((int)JSONGet["led2"]);
          assignParam();
        }
      }else{
        Serial.println(httpCode);
        Serial.println("ERROR on HTTP Request");
      }
    }else{
      WiFi_Connect();
    }
    vTaskDelay(500/tckMs);
  }
}

void _post(void *param){
  while(1){
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
  
      http.begin(url);
      http.addHeader("Content-Type","application/json");
    
      JSONPost["ldr"] = ldrVal;
      JSONPost["led1"] = led1Val;
      JSONPost["led2"] = led2Val;
      serializeJson(JSONPost,str);
      int httpCode = http.POST(str);
  
      if(httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        
      }else{
        Serial.println(httpCode);
        Serial.println("ERROR on HTTP Request");
      }
    }else{
      WiFi_Connect();
    }
     vTaskDelay(500/tckMs);
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
  WiFi.mode(WIFI_STA);
  WiFi_Connect();

  assignParam();

  xTaskCreatePinnedToCore(_get,"get",1024*32,NULL,1,&taskGet,0);
  xTaskCreatePinnedToCore(_post,"post",1024*32,NULL,2,&taskPost,1);
}

void loop(){
  digitalWrite(led1,!digitalRead(led1));
  digitalWrite(led2,!digitalRead(led2));
  vTaskDelay(1000/tckMs);
}


```
