
#include<SoftwareSerial.h>
SoftwareSerial gsm (3,4);
void setup()
{
  // put your setup code here, to run once:
Serial.begin(9600);
gsm.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if(gsm.available())
{
  Serial.write(gsm.read());
 // Serial.write("http://13.235.190.46/insLightData.php/?lid=1&st=" + "");
 
}
if(Serial.available()){
byte a=(Serial.read());
if(a=='#')
{
  gsm.write(0x1a);
  }
 else 
 gsm.write(a); 
}
}
