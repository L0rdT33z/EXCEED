# ETC

## Debounced Switch

ประสิทธิภาพดีกว่า debounced ด้วย delay

### Exercise : Debounced Switch

```
volatile bool state = false;
int sw = 23;

hw_timer_t *timer = NULL;
int i=0;

void debounce(){
  static int count = 20;
  if(digitalRead(sw)){
    count--;
    if(count<0){
      count = 0;
    }
    if(count==0){
      state=true;
    }
  }
  else{
    count++;
    if(count>20){
      count=20;
    }
    if(count==20){
      state=false;
    }
  }
}

void setup(){
  pinMode(sw,INPUT);
  Serial.begin(115200);
  delay(2000);
  //
  
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer,debounce,true);
  timerAlarmWrite(timer,100,true);
  timerAlarmEnable(timer);
  
  //
}



void loop(){
  if(state){
    Serial.println("Pressed!");
    Serial.println(i);
    i++;
    while(state);
  }
  //
  /*
    if(digitalRead(sw)){
        Serial.print("Pressed! ");
        Serial.println(i);
        i++;
        while(digitalRead(sw));
    }
    */
   //
   /*
    Serial.println(digitalRead(sw));
    */
   //
}

```

## Exercise : Touch pin

เป็นเซ็นเซอร์ที่ติดตั้งมาให้บนบอร์ด ESP32 อยู่พินหมายเลข 2 , etc

(ดูเลขพินจาก https://www.espressif.com/sites/default/files/documentation/esp32-wrover-b_datasheet_en.pdf หน้า 9)

```
int touch_pin = 2;
int led = 5;

void setup(){
  pinMode(touch_pin,INPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  Serial.begin(115200);
}

void loop(){
  Serial.println(touchRead(touch_pin));
  if(touchRead(touch_pin)<30){
    digitalWrite(led,HIGH);
  }else{
    digitalWrite(led,LOW);
  }
}

```

## Exercise : Hall sensor

เป็นการอ่านค่าแม่เหล็กบนบอร์ด (อยู่ใต้แผงเหล็ก แต่อาจอ่านค่าไม่ค่อยชัดเจน)

```
void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.print("Value: ");
  Serial.println(hallRead());
  delay(1000);
}

```


