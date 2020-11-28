#include<SoftwareSerial.h>
SoftwareSerial gsm (4,5);    //***** 4 is Reciever and 5 is Transmitter*********/////
long I1samplesum=0;
long I2samplesum=0;
long V1samplesum=0; 
long lastsample=0;
int samplecount=0;             
const int acs1_712=A1;    ///**********************current1 sensor 5amps**********************///      
const int acs2_712=A3;    ///***************************current2 sensor 5 amps*****************************/// 
const int ZMPT101A=A0; 
int vpc=4.88; 
float  voltage;
float  ampere;
int a=0,b=0,c=0, x=0,y=0,z=0;      ///****************condition checker***********************///

void setup()
   {
      Serial.begin(9600);
      gsm.begin(9600);
      Serial.print(".....");
      pinMode(acs1_712,INPUT);
      pinMode(acs2_712,INPUT);
      pinMode(ZMPT101A,INPUT);
      delay(5000);
 
   }
void loop() 
   {
      //Serial.println(analogRead(acs1_712));
      //put your main code here, to run repeatedly:
        V1samplesum+=sq(analogRead(ZMPT101A)-512);
        I1samplesum+=sq(analogRead(acs1_712)-510);       
        I2samplesum+=sq(analogRead(acs2_712)-510); 
        
if(millis()>lastsample)
   {
        lastsample= millis(); 
        samplecount++;   
   }
    
if(samplecount==1000)
  {
         float voltagemean1 = V1samplesum/samplecount;
         float V1rms = sqrt(voltagemean1);
         float V1 = (V1rms*vpc);
         float voltage1=map(V1,0,300,0,10);
         float currentmean1 = I1samplesum/samplecount;  
         float currentmean2 = I2samplesum/samplecount;//RMS value of current and voltage//
         float I1rms = sqrt(currentmean1);
         float I2rms = sqrt(currentmean2);
         float mv1 = (I1rms*vpc);
         float mv2 = (I2rms*vpc);
         float current1 = ((mv1/185*1000));//*1000
         float current2 = ((mv2/185*1000));//*1000
         Serial.println("ampere1 :" +String(current1)+"\tampere2 :" +String(current2)+"\tvoltage1 :" +String(voltage1) ); 
         delay(5000);                                             
            

if (( voltage1<=5 )&& (current1<100)&& (x==0))  ////Both phase are off ////
   {

  
    Serial.print(".......................................Entering both phase off");
      delay(500);
      gsm.println("AT+CFUN=1"); 
      ShowSerialData();
      delay(20000); 
      initializeGPRS();
      senddata("22",voltage1, current1 );
      delay(500);
      x=1;
      y=0;
      z=0;
      }
      
      else if(( voltage1>=6) && (current1>100) && (y==0)){
        gsm.println("AT+CFUN=1"); 
        ShowSerialData();
        delay(20000);
        initializeGPRS();
        senddata("22",voltage1, current1 );
        delay(500);
        y=1;
        x=0;
        z=0;
        }
      else if (( voltage1>=6) && (current1<100) && (z==0)){
        gsm.println("AT+CFUN=1"); 
        ShowSerialData();
        delay(20000);
        initializeGPRS();
        senddata("22",voltage1, current1 );
        delay(500);
        z=1;
        x=0;
        y=0;
        }
        
      if (( voltage1<=5 ) && (current2<100)&& (a==0)) {
        gsm.println("AT+CFUN=1"); 
        ShowSerialData();
        delay(20000);
         initializeGPRS();
         delay(500);
        senddata("23",voltage1, current2 );
        delay(500);
         a=1;
         b=0;
         c=0;
        }
      else if(( voltage1>=6) && (current2>100) && (b==0)){
        gsm.println("AT+CFUN=1"); 
        ShowSerialData();
        delay(20000);
        initializeGPRS();
        senddata("23",voltage1, current2 );
        delay(500);
        b=1;
        a=0;
        c=0;
        }
      else if (( voltage1>=6) && (current2<100) && (c==0)){
        gsm.println("AT+CFUN=1"); 
        ShowSerialData();
        delay(20000);
        initializeGPRS();
        senddata("23",voltage1, current2 );
        delay(500);
        c=1;
        a=0;
        b=0;
 }
   
V1samplesum=0;
I1samplesum=0;
I2samplesum=0;
samplecount=0;
lastsample=0;
delay(5000);
        
  }
  }
  
/////////////////************************Initializing gprs function*******************************//////////////////////
  
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
        
//////////////////////////***************Acknowledgement from GSM*******************////////////////////////////////////////        

void ShowSerialData()

   {
          while(gsm.available()!=0)  /* If data is available on serial port */
          Serial.write(char (gsm.read())); /* Print character received on to the serial monitor */
          delay(2000);
   }
   
///////////////////////*****************Sending data from gsm function***************************/////////////////////////////////////

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
        
        gsm.println("AT+CFUN=0"); 
        ShowSerialData();
  
   }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
