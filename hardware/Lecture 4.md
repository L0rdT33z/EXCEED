# Timer

เป็นองค์ประกอบที่ Integrated มาให้แล้วในบอร์ด ESP32

## Type qualifier

เป็นคีย์เวิร์ดที่กำกับกับตัวแปร เช่น

volatile ใช้กำกับให้ compiler รู้ว่าตัวแปรจะไม่ถูก optimize (เนื่องจาก timer + optimize ทำให้ลำดับเพี้ยน)

const ใช้กำกับให้ค่าของตัวแปรไม่ถูกเปลี่ยนแปลง

## Storage class specifier

เป็นคีย์เวิร์ดที่ใช้นิยามว่าตัวแปรหรือฟังก์ชัน จะมีลักษณะการเก็บข้อมูลอย่างไร

static ถ้าประกาศให้ตัวแปรในฟังก์ชั่น ทำให้ค่าตัวแปรแชร์ระหว่างทุก instance

- ถ้าเป็นตัวแปรแบบ global จะใช้งานได้ภายในไฟล์ที่ประกาศตัวแปรเท่านั้น

## Timer

counter ค่าได้สูงสุด (esp32 ได้ 16 bit) ถึง 2<sup>16</sup>

### Prescaler

เป็นตัวปรับจูนค่า ไม่ให้นับถี่ๆได้ 

freq_timer = freq_clock/PSC

freq_clock - บอร์ดนี้ใช้ 80 MHz

- hw_timer_t \*timer_var = NULL;

- timer_var = timerBegin(id,prescale,flag);
  
  - id -> 0-3 (ESP32 มี Timer 4 ตัว)
  
  - Flag -> Boolean
  
  - True -> Rising edge (ส่วนใหญ่ใช้)
  
  - False -> Falling edge

- timerAttachInterrupt(timer_var, func, trigger);
  
  - Trigger -> Boolean
  
  - true -> Edge
  
  - false -> Level

- timerAlarmWrite(timer_var,freq,autoreload); -> freq (หน่วย micro second เป็นคาบ)
  
  - autoreload -> True (Trigger เรื่อยๆ) , False (ทำรอบเดียวจบ)

- timerAlarmEnable(timer_var); -> ใช้งาน

#### Exercise : Separated LED Blinking with Timer

```
int led1 = 5;
int led2 = 22;

hw_timer_t *timer = NULL;

void blink_fn(){
  digitalWrite(led1,!digitalRead(led1));
}

void setup(){
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  digitalWrite(led1,HIGH);
  digitalWrite(led2,LOW);

  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer,&blink_fn,true);
  timerAlarmWrite(timer,700000,true);
  timerAlarmEnable(timer);
}

void loop(){
  digitalWrite(led2,!digitalRead(led2));
  delay(200);
}

```

### Interrupt Watchdog Timer

Config ไม่ได้ (hardcode มาแล้ว)

คอยดูแล Interrupt ทุกประเภท (ไม่ให้ CPU Time กับ Interrupt มากเกินจนรบกวนการทำงาน Main CPU)

จะ panicked ถ้า interrupt ใช้เวลานานเกินกำหนด

### Task Watchdog Timer

#include <esp_task_wdt.h>

esp_task_wdt_init(timeout, panic)

- Timeout -> sec

- panic -> boolean (true เมื่อหมดเวลาให้ panic)

- panic -> Dump core ทำให้บอร์ดรีบูท

esp_task_wdt_deinit() ยกเลิกไม่ให้ watchdog timer ทำงาน

esp_task_wdt_add(TaskHandle) เพิ่ม Task กับ watchdog timer

esp_task_wdt_reset() - กลับไปนับ time ใหม่

esp_task_wdt_delete(TaskHandle) ลบ Task จาก watchdog timer

#### Exercise : Task WDT

```
#include<esp_task_wdt.h>

void setup(){
  Serial.begin(115200);
  Serial.println("init WDT...");
  esp_task_wdt_init(1,true);
  esp_task_wdt_add(NULL);
}

void loop(){
  for(int i=0;i<5;i++){
    Serial.println("Resetting WDT...");
    esp_task_wdt_reset();
    delay(100);
  }

  while(1){
    Serial.println("Stop WDT reset.");
    delay(100);
  }
}

```
