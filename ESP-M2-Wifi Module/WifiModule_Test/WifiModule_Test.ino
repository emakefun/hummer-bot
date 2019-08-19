byte read_dat;
void setup()
 {
     Serial.begin(9600);
     Serial.println("Wifi Test");
 }

void loop()
 {
     while (Serial.available() > 0)  
     {
         read_dat = Serial.read();
         delay(2);
         Serial.write(read_dat);
     }
 }