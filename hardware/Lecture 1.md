# Intro

ใช้ Arduino (C,C++)

ฺBoard : LOLIN D32 PRO

## Basic IO

### GPIO

- General Purpose Input/Output (พอร์ตอเนกประสงค์)

- ใช้กำหนดเป็น Input หรือ Output ก็ได้

- บอร์ดนี้ใช้ทุกพินที่มีหมายเลขกำกับเป็น GPIO ได้

#### Internal LED

อยู่พินหมายเลข 5 เป็น Active Low (ทำงานเมื่อป้อนค่า Low)

### Function

- pinMode(pin_number,mode) - กำหนดพินนั้นให้เป็นสถานะที่ต้องการ
  
  - INPUT , OUTPUT , INPUT_PULLUP

- digitalWrite(pin_number,value) กำหนดค่าให้พินนั้น high low

- delay(time in ms)

#### Exercise : LED Blinking

- เปิด Arduino จะมีส่วน setup กับ loop

- setup รันรอบเดียว loop ทำงานเรื่อยๆ

```
void setup(){
  pinMode(5 , OUTPUT); // กำหนด Pin 5 เป็น output
}

void loop(){
  digitalWrite(5, LOW); // ทดสอบติดไฟ
  // Tools -> Port เช็คว่าพอร์ตถูกต้อง
   // ก่อนอัพโหลดให้กด Verify เพื่อตรวจก่อน
  // ถ้าขึ้น Hard resetting แปลว่าโหลดลงบอร์ดสำเร็จ

  //ทำให้กะพริบ เพิ่ม
  delay(200); // ดีเลย์ 200 ms    
  digitalWrite(5,HIGH);
  delay(200);
}
```

### Serial

การส่งข้อมูลผ่าน USB (Universal Serial Bus)

ใช้ Debug แสดงข้อมูลมราต้องการ

ควรระวัง baud rate

digitalRead(pin_number) - อ่านค่าจาก pin ที่ต้องการ (return HIGH LOW)

#### Exercise : Button to Serial

การใช้ Bread Board

2 แถว เชื่อมแนวนอน ที่เหลือเชื่อมแบบแนวตั้ง

Button

2 ขาร่าง กับ 2 ขาบนเชื่อมกัน ด้านบนกับด้านล่างเชื่อมถึงกันเมื่อกดปุ่ม

```
void setup(){
  pinMode(22 , INPUT); // กำหนด pin input เป็น pin ที่ 22
  Serial.begin(9600); // กำหนดค่าความถี่ที่ใช้ส่งเป็น 9600 baud rate
}

void loop(){
  Serial.println(digitalRead(22)); //ตรวจสอบข้อมูลที่ได้จากปุ่มกด 
  // println แสดงและเว้นบรรทัด digitalRead อ่านค่า
  delay(10);
}
```

ดูข้อมูล กด Serial Monitor มุมขวาบน

สาเหตุที่กดแล้วไม่ขึ้น 1 ครั้งเดียว เพราะการ bounce ของสวิตช์ การแก้ไขอาจใช้ delay()

เวลาไม่กดปุ่มแต่มีค่า 1 เพราะการต่อ input ต่อได้ 3 แบบคือ

floating (ไม่มีตัวต้านทานในการกำจัด noise)

pulled-up -> pinMode INPUT_PULLUP (ค่าจะกลับ)

pulled-down

#### Exercise: LED Button

เอาแลปแรกกับแลป 2 มารวมกัน

ให้กดปุ่มไฟติด ไม่กดไฟดับ

```
int sw=22;
int led=5;

void setup(){
    pinMode(sw,INPUT_PULLUP);
    pinMode(led,OUTPUT);
    digitalWrite(led,HIGH);
    Serial.begin(9600);
}

void loop(){
    Serial.println(digitalRead(sw));
    digitalWrite(led, digitalRead(sw));
    delay(10);
}
```

### Digital vs Analog

Digital - เป็น Discrete Value (Binary , เลขฐานต่างๆ)

Analog - Continuous Value (ค่าที่มีความต่อเนื่อง เช่น อุณหภูมิ)

analogRead(pin_number) return int

Analog Input - ใช้ได้ 6 PIN (อ่านจาก document)

LDR - Light Dependent Resistor (Module , แบบเปล่าๆ ต้องต่อ Voltage Divider เอา)

#### Exercise : LDR to Serial

- ถ้าได้ Module ต่อเชื่อมบอร์ดได้เลย - เชื่อม ground S เชื่อมกับ pin ที่เรากำหนด 3V เชื่อมกับ 3V ได้เลย

- ถ้าได้แบบเปล่าๆ ต้องต่อตัวต้านทานเป็น Voltage Divider ให้ LDR ต่อกับ ground อีกตัวต่อ PIN แล้วก็ต่อตัวต้านทาน
  
  ```
  int ldr = 34;
  
  void setup(){
      pinMode(ldr,INPUT);
      Serial.begin(9600);
  }
  
  void loop(){
      Serial.println(analogRead(ldr));
      delay(100);
  }
  ```

#### Exerise : Auto LED

ใช้ LDR กับ Internal LED

แสงน้อย LED ติด แสงมาก LED ดับ

```
int led = 5;
int ldr = 34;

void setup(){
    pinMode(led,OUTPUT);
    pinMode(ldr,INPUT);
    Serial.begin(9600);
}

void loop(){
    Serial.println(analogRead(ldr));
    if(analogRead(ldr)>1000)
        digitalWrite(led,LOW);
    else
        digitalWrite(led,HIGH);
    delay(10);
}
```

### PWM

- Pulse-Width Modulation การเข้ารหัสสัญญาณ Digital ให้มี output เฉลี่ยคล้าย Analog

- สร้าง logic HIGH LOW สลับกัน

- บอร์ดนี้มี 16 channel 0-15 เลือกได้ตั้งแต่ 1 bit ถึง 16 bit

- ความถี่สูงสุดใช้ได้ตามสูตร 240 x 10<sup>6</sup> / 2<sup>resolution</sup> Hz

- resolution (บิตอ้างอิง)

- Duty level = 2<sup>resolution</sup> levels

- Function
  
  - ledcSetup(channel, freq, resolution)
  
  - ledcAttachPin(pin_number, channel);
  
  - ledcWrite(channel, duty_cycle);

- Duty cycle
  
  - Duty cycle น้อย ค่า HIGH น้อย 
  
  - Duty cycle มากค่า HIGH มาก

- #### Exercise : LED Fade Blink
  
  ```
  int led = 5;
  int ch = 0;
  int freq = 1000;
  int res = 8;
  
  void setup(){
      pinMode(led,OUTPUT);
      ledcSetup(ch, freq,res);
      ledAttachPin(led, ch);
      ledcWrite(ch,0);
  }
  
  void loop(){
      for(int i=0;i<256;i++){
          ledcWrite(ch,i);
          delay(10);
      }
  }
  ```
