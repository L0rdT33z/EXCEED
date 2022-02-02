int led = 5;
int ch = 0;
int freq = 1000;
int res = 8;

void setup(){
    pinMode(led,OUTPUT);
    ledcSetup(ch, freq,res);
    ledcAttachPin(led,ch);
    ledcWrite(ch,0);
}

void loop(){
    for(int i=0;i<256;i++){
        ledcWrite(ch,i);
        delay(10);
    }
}
