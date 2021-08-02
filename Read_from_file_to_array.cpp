#include "SPIFFS.h"
int count = 0; 
String sData = "";
void setup() {
  Serial.begin(115200);  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/tam.txt"); // mở file 
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  int lengthA = file.size(); // gán độ dài của mảng lưu trong file (theo byte)
  uint16_t arraySend[lengthA]; // gán 1 mảng có độ dài lengthA
  
  Serial.println("File Content:");
  while(file.available()){    // quá trình đọc kí tự
    int textAscii = file.read(); // gắn với kiểu ascii (dec) của kí tự trong file "A" thì textAscii = 49
       if (textAscii == 59) {  // dấu chấm phẩy
         arraySend[count]  = sData.toInt(); // biến đổi thành int vì đang là string       
         Serial.print ( arraySend[count]); //        
//         Serial.print (sData); 
         Serial.print (","); 
         sData = "";
         count += 1;   
       }else{      
        sData += (textAscii-48);        
        }
        
  }
  file.close(); 

  for (int i =0 ; i<count; i++){
  Serial.print("arraySend = ");
  Serial.print(arraySend[i]);
  Serial.print (" ");
  }
}
 
void loop() {

}