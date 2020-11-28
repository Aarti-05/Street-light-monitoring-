#include <GSM.h>

#define PINNUMBER "0,1"


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
;
}

void loop() {
  // put your main code here, to run repeatedly:
if(GSM.available())
{
  Serial.write(gsm.read());
}
if(Serial.available()){
byte a=(Serial.read());
if(a='#')
{
  GSM.write(0x1a);
  }
 else 
 GSM.write(a); 
}
}
