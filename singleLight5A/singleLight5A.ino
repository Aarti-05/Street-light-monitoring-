#include<SoftwareSerial.h>
//SoftwareSerial gsm (3,4);
SoftwareSerial gsm (4,5);
long I1samplesum=0;
long Vsamplesum=0;                               //*********************** sample interval ************************************//
long lastsample=0;
int samplecount=0;                              //****************************** number of samples ********************************//
const int acs1_712=A1;                          //*************************current sensor 30 amps**********************************//       
const int ZMPT101=A0; 
int vpc=4.88; 
float  voltage;
float  ampere;
int w=0, x=0, y=0, z=0;                         //************************* condition checker ********************//


void setup() 
{
Serial.begin(9600);
gsm.begin(9600);
Serial.print(".....");
pinMode(acs1_712,INPUT);
pinMode(ZMPT101,INPUT);
delay(5000);
}

void loop()
{
        Serial.println(analogRead(acs1_712));
        Vsamplesum+=sq(analogRead(ZMPT101)-512);
        I1samplesum+=sq(analogRead(acs1_712)-510);  
       

if(millis()>lastsample)
      {
       lastsample= millis(); 
       samplecount++;   
      }                         
    
if(samplecount==1000)

      {
                                                    
        float voltagemean = Vsamplesum/samplecount;
        float Vrms = sqrt(voltagemean);
        float V = (Vrms*vpc);
        float voltage=map(V,0,300,0,10);
        float currentmean1 = I1samplesum/samplecount;  
       
        float I1rms = sqrt(currentmean1);
       
        float mv1 = (I1rms*vpc);
        float current1 = ((mv1/185*1000));
        Serial.println( current1);
        Serial.println("ampere1 :" +String(current1)+"\tvoltage :" +String(voltage) ); 
        delay(20000);
        
if (( voltage<=5 && (current1<100)  && x==0))

    {
        initializeGPRS();
        senddata("7",voltage, current1 );
        delay(500);
        x=1; 
        y=0; 
        z=0;
       
    }
else if (( voltage>=6)&&(current1>100) && (y==0))   // *******************previous condition current1>=100*************//
   {
        initializeGPRS();
        senddata("7",voltage, current1 );
        delay(500);
        y=1; 
        x=0;
        z=0; 
    }
else if (( voltage>=6)&&(current1<100) && (z==0))   // previous condition current1<=90//
   {
        initializeGPRS();
        senddata("7",voltage, current1 );
        delay(500);
        y=0; 
        x=0; 
        z=1;
   }
   

Vsamplesum=0;
I1samplesum=0;
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
        gsm.println("AT");                 /* Check Communication */
        
        ShowSerialData();  /* Print response on the serial monitor */
        /* Configure bearer profile 1 */
        Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");
        gsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
       
        ShowSerialData(); 
       
        Serial.print("AT+SAPBR=3,1,\"APN\",\"INTERNET\"\\r\\n");
        gsm.println("AT+SAPBR=3,1,\"APN\",\"INTERNET\"");        
        
        ShowSerialData(); 
        Serial.print("AT+SAPBR=1,1\\r\\n");
        gsm.println("AT+SAPBR=1,1");        
       
        ShowSerialData(); 
        Serial.print("AT+SAPBR=2,1\\r\\n");
        gsm.println("AT+SAPBR=2,1");          
        
        ShowSerialData(); 
        Serial.print("AT+HTTPINIT\\r\\n");
        gsm.println("AT+HTTPINIT");          
         
        ShowSerialData(); 
        Serial.println("-----Exiting initializeGPRS function");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowSerialData()
{
        while(gsm.available()!=0)                     /* If data is available on serial port */
        Serial.write(char (gsm.read()));              /* Print character received on to the serial monitor */
        delay(2000);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
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
