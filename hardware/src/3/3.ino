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
