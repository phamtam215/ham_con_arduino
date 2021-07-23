#define but1 23
#define but2 21
int count1 = 0;
int count2 = 0;
unsigned long last;
void setup() {
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  Serial.begin (115200);
  last = millis();
}
void loop() {
  if (digitalRead(but1) == 0)
  {
    delay(200);
    count1 ++;
    Serial.print ("count1 = ");
    Serial.println (count1);
  }
  if (digitalRead(but2) == 0)
  {
    if ( (millis() - last) > 100){
    count2 ++;
    Serial.print ("count2 = ");
    Serial.println (count2);
    }
    last = millis();
  }
  
}
