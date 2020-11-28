const int ZMPT101=A0;
long samplesum=0;
long Vsamplesum=0;              // sample interval//
long lastsample=0;
int samplecount=0;              // number of samples//
const int acs712=A1;          //current sensor input//
int vpc=4.88; 
float  voltage;
float  ampere;
int x=0, y=0, z=0;      // condition checker
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println( analogRead(ZMPT101));
 //delay(1000);
 Vsamplesum+=sq(analogRead(ZMPT101)-512);
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
    // Serial.println(V);
     float voltage=map(V,0,300,0,10);
     Serial.println(V);
     Serial.println(voltage);
     
      Vsamplesum=0;
      samplesum=0;
      samplecount=0;
      delay(3000);
      }
      }
