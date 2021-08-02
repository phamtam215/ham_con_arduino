#define button 0
//#define button2 18
int numPress = 0;
bool isPress = false;
void IRAM_ATTR control() {
  for (int i =0; i <200; i++){
  Serial.printf ("Button1 được bấm %u times\n", i);
  yield();
  }
}
void setup () {
  Serial.begin (115200);
  pinMode (button, INPUT_PULLUP);
//  pinMode (button2, INPUT_PULLUP);
  attachInterrupt (button, control, FALLING);
  //attachInterrupt (button2, isr, FALLING);
}
void loop () {
}
