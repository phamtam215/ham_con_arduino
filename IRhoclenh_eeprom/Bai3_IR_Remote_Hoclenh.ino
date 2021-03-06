2#include <IRremote.h>
#include "Button.h"
#include <EEPROM.h>
const int RECV_PIN = 14;
const int pinRelay1 = 2;
const int pinRelay2 = 4;
//const int pinRelay3 = 5;
//const int pinRelay4 = 6;
const int pinLearning = 21;
const int BT_PIN = 18; 

unsigned long lastTime = millis();

uint32_t dataRemote1;
uint32_t dataRemote2;
//uint32_t dataRemote3;
//uint32_t dataRemote4;

bool learning = false;
int indexRemote=0;
         
#define PULLUP true        
#define INVERT true        
#define DEBOUNCE_MS 20     

Button btnLearn(BT_PIN, PULLUP, INVERT, DEBOUNCE_MS);
//=============================================
IRrecv irrecv(RECV_PIN);
decode_results results;
//=============================================
void setup(){
  Serial.begin (115200);
  EEPROM.begin(512);
  pinMode(pinRelay1,OUTPUT);
  pinMode(pinRelay2,OUTPUT);
//  pinMode(pinRelay3,OUTPUT); 
//  pinMode(pinRelay4,OUTPUT);
  pinMode(pinLearning,OUTPUT);
  digitalWrite(pinRelay1, LOW);
  digitalWrite(pinRelay2, LOW);
//  digitalWrite(pinRelay3, LOW);
//  digitalWrite(pinRelay4, LOW);
  digitalWrite(pinLearning, LOW);
  
  //Khởi động bộ thu
  //irrecv.blink13(true);
  irrecv.enableIRIn();
  
  //Lấy dữ liệu remote từ EEPROM
  dataRemote1 = get_remote_eeprom(1);
  dataRemote2 = get_remote_eeprom(2);
//  dataRemote3 = get_remote_eeprom(3);
//  dataRemote4 = get_remote_eeprom(4);
  Serial.println(dataRemote1);
  Serial.println(dataRemote2);
//  Serial.println(dataRemote3);
//  Serial.println(dataRemote4);
}
//=============================================
void loop() {
  btnLearn.read();
  if (irrecv.decode(&results)) {
    uint32_t dataRemote = results.value;
    if(dataRemote > 0 && dataRemote != REPEAT){
      if (millis() - lastTime > 250) {
        if(!learning){
          if(dataRemote == dataRemote1){
            digitalWrite(pinRelay1, !digitalRead(pinRelay1));
          }else if(dataRemote == dataRemote2){
            digitalWrite(pinRelay2, !digitalRead(pinRelay2));
          }
//          }else if(dataRemote == dataRemote3){
//            digitalWrite(pinRelay3, !digitalRead(pinRelay3));
//          }else if(dataRemote == dataRemote4){
//            digitalWrite(pinRelay4, !digitalRead(pinRelay4));
//          }
        }
        else{
          save_remote_eeprom(indexRemote,dataRemote);
        }
      }
    }
    lastTime = millis();
    irrecv.resume(); // Cho phép nhận giá trị tiếp theo
  }
  if(btnLearn.wasPressed()){
    if(!learning){
      learning=true;
      indexRemote=1;
      digitalWrite(pinLearning, HIGH);
      digitalWrite(pinRelay1, HIGH);
      digitalWrite(pinRelay2, LOW);
//      digitalWrite(pinRelay3, LOW);
//      digitalWrite(pinRelay4, LOW);
    }
    else{
      indexRemote++;
      switch(indexRemote){
        case 1:
          digitalWrite(pinRelay1, HIGH);
          digitalWrite(pinRelay2, LOW);
//          digitalWrite(pinRelay3, LOW);
//          digitalWrite(pinRelay4, LOW);
        break;
        case 2:
          digitalWrite(pinRelay1, LOW);
          digitalWrite(pinRelay2, HIGH);
//          digitalWrite(pinRelay3, LOW);
//          digitalWrite(pinRelay4, LOW);
        break;
//        case 3:
//          digitalWrite(pinRelay1, LOW);
//          digitalWrite(pinRelay2, LOW);
////          digitalWrite(pinRelay3, HIGH);
////          digitalWrite(pinRelay4, LOW);
//        break;
//        case 4:
//          digitalWrite(pinRelay1, LOW);
//          digitalWrite(pinRelay2, LOW);
////          digitalWrite(pinRelay3, LOW);
////          digitalWrite(pinRelay4, HIGH);
//        break;
      }
      if(indexRemote>2){
        learning=false;
        dataRemote1 = get_remote_eeprom(1);
        dataRemote2 = get_remote_eeprom(2);
//        dataRemote3 = get_remote_eeprom(3);
//        dataRemote4 = get_remote_eeprom(4);
        digitalWrite(pinLearning, LOW);
        digitalWrite(pinRelay1, LOW);
        digitalWrite(pinRelay2, LOW);
//        digitalWrite(pinRelay3, LOW);
//        digitalWrite(pinRelay4, LOW);
      }
    }
  }
}
//=====================================
void save_remote_eeprom(int pos, uint32_t data){
  int address = pos*4;
  EEPROM.write(address,((byte *)&data)[0]);
  EEPROM.write(address+1,((byte *)&data)[1]);
//  EEPROM.write(address+2,((byte *)&data)[2]);
//  EEPROM.write(address+3,((byte *)&data)[3]);
}
//=====================================
uint32_t get_remote_eeprom(int pos){
  int address = pos*4;
  uint32_t data = 0;
  ((byte *)&data)[0] = EEPROM.read(address);
  ((byte *)&data)[1] = EEPROM.read(address+1);
//  ((byte *)&data)[2] = EEPROM.read(address+2);
//  ((byte *)&data)[3] = EEPROM.read(address+3);
  return data;
}
