void setup() {
  // put your setup code here, to run once:
  pinMode(5 , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(5, LOW);
  delay(200);   
  digitalWrite(5,HIGH);
  delay(200);
}