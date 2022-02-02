# MultiTasking

ทำให้โปรแกรมทำงานหลายอย่างพร้อมกันได้

## Pseudo Multitasking

multitasking ปลอมๆ

Using millis()

เรียกการทำงานแบบ Super Loop

Entry Point -> Setup -> Loop (Infinite)

การใช้ฟังก์ชัน millis() จะ return ตัวเลขเป็นเวลาในหน่วย ms ตั้งแต่บอร์ดเปิด

(มี overflow กลับเป็น 0 ประมาณ 50 วัน)

ให้หลอดไฟ 2 หลอดกะพริบด้วยความเร็วที่ต่างกัน

```
#define LED-R 4
#define LED_Y 5

unsigned long r_ts, y_ts;

void setup(){
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, LOW);
}

void loop(){
    if(millis() - r_ts > 500){
            digitalWrite(LED_R, !digitalRead(LED_R));
            r_ts = millis();    
    }

    if(millis() - y_ts > 2000){
            digitalWrite(LED_R, !digitalRead(LED_R));
            y_ts = millis();    
    }
}
```

## Real Multitasking

Multitasking แบบจริงๆ

ใช้ RTOS (Real-time Operating System)

ในที่นี้จะใช้ FreeRTOS

Setup -> สร้าง Task (เขียน parallel ได้)

### FreeRTOS

#### Create a task

xTaskCreatePinnedToCore(Task,Task_name,Stack_Size,Param,Priority,Task_Handle,Core_ID);

#### Delay

vTaskDelay(Tick);

vTaskDelay(ms/portTICK_PERIOD_MS);

#### Task handler

vTaskSuspend(Task_Handle)

vTaskResume(Task_Handle)

vTaskDelete(Task_Handle)

uxTaskPriorityGet(Task_Handle)

vTaskPrioritySet(Task_Handle,priority)

#### Exercise

```
#define LED_R 4


void setup(){
    pinMode(LED_R, OUTPUT);
    xTaskCreatePinnedToCore(
        Task A,
        "Task A",
        1024    ,
        NULL,
        1,
        NULL,
        1
    );
}

void TaskA(void *parameter){
    while(1){
        digitalWrite(LED_R, !digitalRead(LED_R));
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void loop(){

}
```

```
#define LED_R 4
#define LED_Y 5

void setup(){
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT); 

    xTaskCreatePinnedToCore(
        TaskA,
        "Task A",
        1024    ,
        NULL,
        1,
        NULL,
        1
    );

   xTaskCreatePinnedToCore(
        TaskB,
        "Task B",
        1024    ,
        NULL,
        1,
        NULL,
        1
    );

}

void TaskA(void *parameter){
    while(1){
        digitalWrite(LED_R, !digitalRead(LED_R));
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void TaskB(void *parameter){
    while(1){
        digitalWrite(LED_R, !digitalRead(LED_Y));
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}

void loop(){
    
}
```

### FreeRTOS Scheduler

การทำงานของ RTOS จะแบ่งการประมวลผลออกเป็นช่วงเวลา เรียกว่า Tick

และแบ่งลำดับความสำคัญของงาน ESP32 ตั้ง priority ได้ตั้งแต่ 0-24

Hardware Interrupt มี Priority มากสุด

#### Exercise

```
void setup(){
    Serial.begin(300);
    vTaskDelay(500/portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(
        TaskA,
        "Task A",
        1024    ,
        NULL,
        1,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        TaskB,
        "Task B",
        1024    ,
        NULL,
        1,
        NULL,
        0
    );

}

void loop(){



}
```

```
void TaskA(void *parameter){
    while(1){
       for(int i=0;i<50;i++){
            Serial.print("*");
        }
    Serial.println("A");
    vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void TaskB(void *parameter){
    while(1){
       for(int i=0;i<50;i++){
            Serial.print("O");
        }
    Serial.println("B");
    vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
```

ลองเปลี่ยน priority ของ task A เป็น 2 แล้วลองลบ delay TaskA ออก

จะสังเกตว่า Task B ไม่มีโอกาสได้ทำงาน เพราะ priority TaskA สูงกว่า

ถ้าอยากให้ taskA ทำงานใส่ delay ตัว Scheduler จะสลับไปทำงาน taskB (เป็นช่วงที่ A delay)

### Task States

Task Creation -> Ready -> <- Running

If delay -> Blocked -> Ready

vTaskSuspend() -> Suspended -> vTaskResume() -> Ready

#### Exercise : ควบคุม delay หลอดไฟ ผ่าน Serial Monitor

```
#define LED_R 4

TaskHandle_t blinkTaskHandle = NULL;

void setup(){
    Serial.begin(115200);

    pinMode(LED_R, OUTPUT);
    digitalWrite(LED_R, LOW);
}

void loop(){
    while(!Serial.available()){
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
    int interval = Serial.parseInt();
    if(interval > 0){
        Serial.println(interval);
        startBlinkTask(interval);
    }
}

void startBlickTask(int interval){
    if(blinkTaskHandle){
        vTaskDelete(blinkTaskHandle);
    }
    xTaskCreatePinnedToCore(
        blinkLEDTask,
        "Blink LED",
        1024,
        (void *)interval,
        1,
        &blinkTaskHandle,
        1
    );
}

void binkLEDTask(void* parameter){
    int interval = (int) parameter;
    while(1){
        digitalWrite(LED_R, !(digitalRead(LED_R)));
        vTaskDelay(interval/portTICK_PERIOD_MS);
    }    

}
```
