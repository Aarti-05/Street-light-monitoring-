#include<SoftwareSerial.h>
SoftwareSerial gsm (4,5);
long I1samplesum=0;
long I2samplesum=0;
long V1samplesum=0; 
long V2samplesum=0;// sample interval//
long lastsample=0;
int samplecount=0;              // number of samples//
const int acs1_712=A1;  //current sensor 5amps///        //current sensor input//
const int acs2_712=A3;  //current sensor 5 amps/// 
const int ZMPT101A=A0; //A1 and A0 are on same phase
const int ZMPT101B=A7; //A3 and A7 are on same phase
int vpc=4.88; 
float  voltage;
float  ampere;
int a=0,b=0,c=0, x=0, y=0, z=0;      // condition checker


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm.begin(9600);
Serial.print(".....");
pinMode(acs1_712,INPUT);
pinMode(acs2_712,INPUT);
pinMode(ZMPT101A,INPUT);
pinMode(ZMPT101B,INPUT);

delay(5000);
}

void loop() {
  //Serial.println(analogRead(acs1_712));
  // put your main code here, to run repeatedly:
V1samplesum+=sq(analogRead(ZMPT101A)-512);
V2samplesum+=sq(analogRead(ZMPT101B)-512);
I1samplesum+=sq(analogRead(acs1_712)-510);       
I2samplesum+=sq(analogRead(acs2_712)-510); 
if(millis()>lastsample)
      {
        
                                 //Serial.println("enteringmilis");
      lastsample= millis(); 
      samplecount++;                  // increment samples//
        
      }
    
     if(samplecount==1000)
      {
                                                   // Serial.println("entering1000"); 
  float voltagemean1 = V1samplesum/samplecount;
  float voltagemean2 = V2samplesum/samplecount;
  float V1rms = sqrt(voltagemean1);
  float V2rms = sqrt(voltagemean2);
  float V1 = (V1rms*vpc);
  float V2 = (V2rms*vpc);
  float voltage1=map(V1,0,300,0,10);
  float voltage2=map(V2,0,300,0,10);
  float currentmean1 = I1samplesum/samplecount;  
  float currentmean2 = I2samplesum/samplecount;//RMS value of current and voltage//
  float I1rms = sqrt(currentmean1);
  float I2rms = sqrt(currentmean2);
  float mv1 = (I1rms*vpc);
  float mv2 = (I2rms*vpc);
  float current1 = ((mv1/185*1000));//*1000
  float current2 = ((mv2/185*1000));//*1000
  //float ampere1= map(current1,0,300,0,7);
  //float ampere2= map(current2,0,300,0,7);
//Serial.println( current1);
//  Serial.println( current2);
 Serial.println("ampere1 :" +String(current1)+"\tampere2 :" +String(current2)+"\tvoltage1 :" +String(voltage1)+"\tvoltage2 :" +String(voltage2) ); //+
  delay(5000);
//   if (( voltage1<=5 && ( voltage2<=5)&& x==0))  ////Both phase are off ////
//   {
//
//  
//    Serial.print(".......................................Entering both phase off");
//      delay(500);
//      initializeGPRS();
//      senddata("22",voltage1, current1 );
//      delay(500);
//      senddata("23",voltage2, current2 );
//      delay(500);
//      x=1;
//      a=0;
//      b=0;
//      c=0;
//      y=0;
//      z=0;
//      }
//    
//  else if (( voltage1>=6)&&(voltage2>=6) && (y==0)) //// Both phase are on/////
//  
//  {
//     
//    Serial.println(".......................................Entering both phase ON");
//    delay(1000);
//      initializeGPRS();
//      senddata("22",voltage1, current1 );
//      delay(500);
//      senddata("23",voltage2, current2 );
//      delay(500);
//      y=1;
//      x=0;
//      z=0;
//      a=0;
//      b=0;
//      c=0;
//      }
//      else if (( voltage1>=6)&&((voltage2<=5)) && (z==0))   ////Phase 1 is on and Phase2 is off///
//      {
//       Serial.println(".......................................Entering One phase1 ON One Phase2 off");
//       delay(1000); 
//      initializeGPRS();
//      senddata("22",voltage1, current1 );
//      delay(500);
//      senddata("23",voltage1, current1 );
//      delay(500);
//      
//      
//      z=1;
//      x=0;
//      y=0;
//      a=0;
//      b=0;
//      c=0;
//   }
//  else if (( voltage1<=5)&&( voltage2>=6) && (c==0)) /// Phase1 is off and Phase2 is on////
//  
// {
//      Serial.println(".......................................Entering One phase1 OFF One Phase1 ON");
//       delay(1000); 
//     initializeGPRS();
//      senddata("22",voltage1, current1 );
//      delay(500);
//      senddata("23",voltage1, current1 );
//      delay(500);
//      
//      z=0;
//      x=0;
//      y=0;
//      a=0;
//      b=0;
//      c=1;
//    }
//   
//      else if (( voltage1>=6)&&(current1<100) && (a==0)) //Phase1 is on and load 1 is off///
//      {
//        Serial.println(".......................................Entering  phase1 ON  Light1 OFF  ");
//       delay(1000); 
//      initializeGPRS();
//      senddata("22",voltage1, current1 );
//      delay(500);
//      
//      a=1;
//  
//     
//      
//   }
//  
//   else if (( voltage2>=6)&&(current2<100) && (b==0))   /// Phase2 on & load 2 is off///
//   {
//       Serial.println(".......................................Entering  phase2 ON  Light2 OFF  ");
//       delay(1000); 
//      initializeGPRS();
//      senddata("23",voltage2, current2 );
//      delay(500);
//      
//     
//      b=1;
//     
//   }

 if (( voltage1<=5 )&& (x==0))  ////Both phase are off ////
   {

  
    Serial.print(".......................................Entering both phase off");
      delay(500);
      initializeGPRS();
      senddata("22",voltage1, current1 );
      delay(500);
      x=1;
      y=0;
      z=0;
      }
      
      else if(( voltage1>=6) && (current1>100) && (y==0)){
        initializeGPRS();
        senddata("22",voltage1, current1 );
        delay(500);
        y=1;
        x=0;
        z=0;
        }
      else if (( voltage1>=6) && (current1<100) && (z==0)){
        initializeGPRS();
        senddata("22",voltage1, current1 );
        delay(500);
        z=1;
        x=0;
        y=0;
        }
        
      if (( voltage2<=5 )&& (a==0)) {
        senddata("23",voltage2, current2 );
        delay(500);
         a=1;
         b=0;
         c=0;
        }
      else if(( voltage1>=6) && (current2>100) && (b==0)){
        initializeGPRS();
        senddata("23",voltage2, current2 );
        delay(500);
        b=1;
        a=0;
        c=0;
        }
      else if (( voltage2>=6) && (current2<100) && (c==0)){
        initializeGPRS();
        senddata("23",voltage2, current2 );
        delay(500);
        c=1;
        a=0;
        b=0;
 }

        
  V1samplesum=0;
  V2samplesum=0;
  I1samplesum=0;
  I2samplesum=0;
  samplecount=0;
  lastsample=0;
  delay(5000);
  }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initializeGPRS()
{
  Serial.println("-----Entered initializeGPRS function");
  Serial.println("HTTP post method :");
  Serial.print("AT\\r\\n");
  gsm.println("AT"); /* Check Communication */
  
  ShowSerialData();  /* Print response on the serial monitor */
  /* Configure bearer profile 1 */
  Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");
  gsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
 
  ShowSerialData(); 
 
  Serial.print("AT+SAPBR=3,1,\"APN\",\"INTERNET\"\\r\\n");
  gsm.println("AT+SAPBR=3,1,\"APN\",\"INTERNET\"");  /* APN of the provider */
  
  ShowSerialData(); 
  Serial.print("AT+SAPBR=1,1\\r\\n");
  gsm.println("AT+SAPBR=1,1"); /* Open GPRS context */
 
  ShowSerialData(); 
  Serial.print("AT+SAPBR=2,1\\r\\n");
  gsm.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  
  ShowSerialData(); 
  Serial.print("AT+HTTPINIT\\r\\n");
  gsm.println("AT+HTTPINIT");  /* Initialize HTTP service */
   
  ShowSerialData(); 
  Serial.println("-----Exiting initializeGPRS function");
}
void ShowSerialData()
{
  while(gsm.available()!=0)  /* If data is available on serial port */
  Serial.write(char (gsm.read())); /* Print character received on to the serial monitor */
  delay(2000);
}
///////////////////////////////////////////
void senddata(String lid,float volt, float amp)
{
  Serial.println("--------------------Entered Send Data function");
  Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
  gsm.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  ShowSerialData();
 
  Serial.print("AT+HTTPPARA=\"URL\",\"http://15.206.26.82/insLightDataNew.php/?lid=" + lid + "&vst=" +String(volt)+ "&cst=" +String(amp)+ "\"\r\n");
  gsm.println("AT+HTTPPARA=\"URL\",\"http://15.206.26.82/insLightDataNew.php/?lid=" + String(lid) + "&vst=" +String(volt)+ "&cst=" +String(amp)+ "\"\r\n");  // Set parameters for HTTP session
 ShowSerialData();
 
  Serial.print("AT+HTTPACTION=1\\r\\n");
  gsm.println("AT+HTTPACTION=1");  /* Start POST session */
  ShowSerialData();
  
  Serial.print("AT+HTTPREAD\\r\\n");
  gsm.println("AT+HTTPREAD"); 
  ShowSerialData();
  
}
