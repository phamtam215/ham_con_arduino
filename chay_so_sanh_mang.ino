uint32_t Raw1 [68] = {3320, 9258,  4522, 584,  600, 602,  608, 548,  600, 606,  606, 598,  574, 576,  602, 600,  570, 582,  574, 628,  1616, 638,  1698, 558,  1704, 550,  1690, 610,  1640, 614,  1622, 636,  1702, 550,  1694, 560,  598, 606,  592, 610,  1618, 588,  1720, 534,  648, 552,  600, 604,  572, 580,  650, 552,  1700, 554,  1700, 606,  602, 550,  604, 552,  1694, 560,  1748, 554,  1698, 564,  1702, 111};
uint32_t Raw2 [68] = {3276,9300,4500,600,550,650,550,600,550,650,550,650,550,600,550,650,550,600,550,650,1600,650,1650,600,1650,600,1650,650,1600,650,1600,650,1650,600,1650,600,550,650,550,650,1600,600,1650,600,600,600,550,650,550,600,600,600,1650,600,1650,650,550,600,550,600,1650,600,1700,600,1650,600,1650,650};
uint16_t Raw3 = 3276750;
unsigned long last = millis();
int count =0;
void setup() {
Serial.begin (115200);

}

void loop() {

//Serial.println (Raw3);
for (int i =0; i<68; i++){
  Serial.print (Raw1[i]);
  Serial.print (" ");
  Serial.print (Raw2[i]);
  Serial.println ("");
  delay (100);
  }
  Serial.println ("hetfor");
  
}

//delay (200);

//}
