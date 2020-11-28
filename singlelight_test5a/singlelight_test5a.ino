#include<SoftwareSerial.h>
//SoftwareSerial gsm (3,4);
SoftwareSerial gsm (4,5);
long I1samplesum=0;
long Vsamplesum=0;              // sample interval//
long lastsample=0;
int samplecount=0;              // number of samples//
const int acs1_712=A1;  
const int acs2_712=A3; //current sensor 30 amps///        //current sensor input//
const int ZMPT101=A0; 
int vpc=4.88; 
float  voltage;
float  ampere;
int w=0, x=0, y=0, z=0;      // condition checker


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm.begin(9600);
Serial.print(".....");
pinMode(acs1_712,INPUT);
pinMode(ZMPT101,INPUT);
pinMode(acs2_712,INPUT);

delay(5000);
}

void loop() {
Serial.println(analogRead(acs1_712));
  // put your main code here, to run repeatedly:
Vsamplesum+=sq(analogRead(ZMPT101)-512);
I1samplesum+=sq(analogRead(acs1_712)-510);       

if(millis()>lastsample)
      {
        
                                 //Serial.println("enteringmilis");
      lastsample= millis(); 
      samplecount++;                  // increment samples//
        
      }
    
     if(samplecount==1000)
      {
                                                   // Serial.println("entering1000"); 
  float voltagemean = Vsamplesum/samplecount;
  float Vrms = sqrt(voltagemean);
  float V = (Vrms*vpc);
  float voltage=map(V,0,300,0,10);
  float currentmean1 = I1samplesum/samplecount;  
 
  float I1rms = sqrt(currentmean1);
 
  float mv1 = (I1rms*vpc);
  float current1 = ((mv1/185*1000));
  Serial.println( current1);
  Serial.println("ampere1 :" +String(current1)+"\tvoltage :" +String(voltage) ); //+
  Vsamplesum=0;
  I1samplesum=0;
  samplecount=0;
  lastsample=0;
  delay(5000);
  }
  }      
