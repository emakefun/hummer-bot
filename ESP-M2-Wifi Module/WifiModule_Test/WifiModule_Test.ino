byte comdata;

void setup()
 {
     Serial.begin(9600);
     Serial.println("Wifi Test");
 }

void loop()
 {
     while (Serial.available() > 0)  
     {
         comdata = Serial.read();
         delay(2);
         Serial.write(comdata);
     }
 }
