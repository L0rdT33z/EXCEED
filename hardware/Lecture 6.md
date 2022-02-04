# DAC

Digital to analog converter

บอร์ดนี้มี 2 ช่องที่ใช้ DAC ได้คือ GPIO 25,26

dacWrite(DAC_PIN,Value)

Value : 0-255

OUTPUT

0 = 0 V

255 = 3.3V

## Exercise : Fade LED

```
#define LED_Y 25

void setup(){
  pinMode(LED_Y, OUTPUT);
}

void loop(){
  for(int i=0;i<256;i++){
    dacWrite(LED_Y,i);
    delay(20);
  }

  for(int i=255;i>=0;i--){
    dacWrite(LED_Y,i);
    delay(20);
  }
}

```
