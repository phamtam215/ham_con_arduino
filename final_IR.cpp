// After send IR signal to control device, need about 5 -7s to active Learning mode 
//there are noises after transmitting, this will ends about 5-10 seconds after sending (shouldn't active Learning mode)
#include "SPIFFS.h"
#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRac.h>
#include <IRutils.h>

#define But_1 0
#define But_2 18
#define ledStatus 22


String filename = "/test.txt"; // will be handled when received message from server
bool checkBut1 = false;
bool checkBut2 = false;
bool checkLearn = false;
const uint16_t SendPin = 4;
const uint16_t RecvPin = 14;
// const uint32_t kBaudRate = 115200;
//use a larger than normal buffer so we can handle AC raw
const uint16_t kCaptureBufferSize = 1024; 
// set threshold to ignore messages with less than minimum on or off pulses.
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%
// kTimeout is milli-Seconds of no-more-data before we consider a message ended.
#if DECODE_AC
// A value this large may swallow repeats of some protocols (), eg. Kelvinator have gaps in their protocols of ~40ms. 
const uint8_t kTimeout = 50;
#else   // DECODE_AC
//Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
IRrecv irrecv(RecvPin, kCaptureBufferSize, kTimeout, true);
IRsend irsend(SendPin);
decode_results results;  // Somewhere to store the results

void IRAM_ATTR checkbutton1 () {
 checkBut1 = true;
}

void IRAM_ATTR checkbutton2 () {
 checkBut2 = true;
}
int takeSizeArray (String filename) {
 if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");  
  }    
  File file = SPIFFS.open(filename);    
  if(!file) {
    Serial.println("Failed to open file for reading");
  }
  int sizeArray;  
  String sData = "";  
  // find the size of array by read a first number of file 
   while(file.available()){    
    // "1" => textAscii = 49
    int textAscii = file.read(); 
        // ";" = Ascii 59
       if (textAscii == 59) { 
         // convert string to int       
         sizeArray = sData.toInt(); 
         Serial.printf ("\n Size of array: %d\n", sizeArray);
         break;// out while after take a first number of file         
       } 
         else {      
        // "1" textAscii = 49 => sData = 49 - 48 = 1    
        sData += (textAscii-48);   
       }        
    } 
  return (sizeArray);
}
// save array to file from learningIr(), the first number of file is the size of array
bool saveIR_toSpiffs(String filename, uint16_t* irArray) { 
   Serial.printf("\nsaveIR_toSpiffs - The content of file\n");
   File file = SPIFFS.open(filename, "w");
   if (!file){
       Serial.printf ("\nError opening file for writting\n");
   return false;
   }
   //irArray = learningIr ();   
   //size of array is the first element of array = *irArray       
   for (int i = 0; i < *irArray; i++) { 
    file.print(*(irArray +i));
    Serial.printf("%d; ",*(irArray +i));
    file.print(";");          
    }      
    if (!file.size()) {
    return false;    
    }
    file.close();
    return true;
}
uint16_t* readIR_fromSpiffs (String filename){
    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");  
     }    
    File file = SPIFFS.open(filename);    
    if(!file) {
    Serial.println("Failed to open file for reading");
     }
    int sizeArray = takeSizeArray (filename); 
    uint16_t arraySend [sizeArray];
    int count = 0;
    String sData = "";    
    uint16_t *irArray;
    irArray = (uint16_t*)malloc( sizeArray*sizeof (uint16_t)); 
    // after take a first number of file and assign for sizeArray
    // continue read file and save next numbers to array
    // this file contain the first number is a size of array, so you should save the second number in array when count = 1
    while(file.available()){     
    int textAscii = file.read(); 
       if (textAscii == 59) {     
          if (count > 0){  
        // start save data when count = 1 (ignore first number)// notice count-1 
          *(irArray+count-1)  = sData.toInt(); 
          Serial.print (*(irArray+count-1));
          Serial.print (", ");  
          }    
          sData = "";      
          count ++;                          
       } else {            
        sData += (textAscii-48);   
       }        
    }
    file.close(); 
    //return an array need be send to the device
    return irArray;
}
//
void control(String filename) { // take array returned from readIR_fromSpiffs
    uint16_t *irArray;
    irArray = readIR_fromSpiffs (filename);
    int sizeArray = takeSizeArray (filename);
    // for (int i = 0; i < sizeArray; i++){
    //     Serial.print (*(irArray +i));
    //     Serial.print ("/ ");
    // }    
    // Serial.printf ("\nsize array = %d",sizeArray);
    irsend.sendRaw(irArray, sizeArray, 38);  
}

uint16_t *learningIr() { // receive data from remote and return Raw array    
    //Serial.printf ("\ngọi hàm learningIR\n");
    digitalWrite (ledStatus, HIGH);
    Serial.println ("Please press the button on remote to learn...");
    while (!checkLearn) {
         if (irrecv.decode(&results)) {
              // rawlen = lenght of ir signal
              int sizeArray = results.rawlen;
              uint16_t *arrayLearning;
              // create an dynamic array, pointed by a pointer =>*arrayLearning represent this array
              arrayLearning = (uint16_t*)malloc( sizeArray*sizeof (uint16_t)); // khi khai báo malloc đảm bảo là phần tử cuối cùng phải có giá trị
           // rawbuf[0] is a noise, so you should start save at rawbuf[1]
           Serial.printf("\nRaw array received\n");
           for (int i = 1; i < sizeArray; i ++) {//rawbuf[0] bị lỗi nên lưu thì rawbuf[1] đầu tiên, vì thế tận dụng luôn phẩn tử đầu tiên để lưu giá trị của mảng
              *(arrayLearning+i)= results.rawbuf[i]+results.rawbuf[i];
              Serial.print(*(arrayLearning+i));
              Serial.print (", ");
           }
            // save the first element is the size of array
              *(arrayLearning) = sizeArray; // lưu phần tử đầu tiên của mảng là kích thước của mảng
              Serial.printf("\nSize of array received: %d\n",*arrayLearning);                   
             //Serial.printf("\nSize of array: %d\n",*arrayLearning);                   
              //return an array the first cell is value of the size of array
              return arrayLearning;
     }
   }
}
void setup() {
  Serial.begin(115200);    
  #if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
  #endif  // DECODE_HASH
  // Override the default tolerance.
  irsend.begin(); 
  irrecv.setTolerance(kTolerancePercentage);  
  irrecv.enableIRIn(); 
  SPIFFS.begin(true);
  pinMode (ledStatus, OUTPUT);
  digitalWrite(ledStatus, LOW);
  pinMode (But_1, INPUT_PULLUP);
  pinMode (But_2, INPUT_PULLUP);
  attachInterrupt (But_1, checkbutton1, FALLING); 
  attachInterrupt (But_2, checkbutton2, FALLING); 
}
 
void loop() {
  if (checkBut1 == true){
    uint16_t *arrayLearning = learningIr ();
    saveIR_toSpiffs(filename, arrayLearning);    
    digitalWrite (ledStatus, LOW);          
    checkBut1 = false;
    free(arrayLearning);
    arrayLearning = NULL;    
 }    
 if (checkBut2 == true){
    Serial.printf ("\nControl Device\n");        
    control(filename);    
    checkBut2 = false;    
  }
}