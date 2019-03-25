String comdata = "";

void setup()
 {
     Serial.begin(9600);
 }

void loop()
 {
  //  Serial.println("Hello");
     while (Serial.available() > 0)  
     {
         comdata += char(Serial.read());
         delay(2);
        // Serial.println(comdata);
     }
     if (comdata.length() > 0)
     {
         Serial.println(comdata);
         comdata = "";
     }
 }
