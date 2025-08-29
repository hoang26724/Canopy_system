#include <Arduino.h>
#define BLYNK_AUTH_TOKEN "EZkotyIG8rllob4xPU5WOkWnJWIlLpYy"
#define BLYNK_TEMPLATE_ID "TMPL68Z8dGj0Q"
#define BLYNK_TEMPLATE_NAME "nhung"
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

#include <WiFiClient.h>
#include "WiFi.h"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Beelz's Galaxy A23";
char pass[] = "huybeelz";

#include <Adafruit_Sensor.h>
#include "DHT.h"
#define Dht_pin 15
#define dht_type DHT22
DHT dht(Dht_pin,dht_type);

#define rainsensor 35
#define switch2 27
#define switch1 26
#define motor 4
#define  inout 2
#define button 33
int chieudc=0;
int onoff=0;
int cbn;
int cbm;
unsigned long count,countled;
bool rainreg=false;
bool rainnext=false;
bool tempreg=false;
bool tempnext=false;

WidgetLED LED_ON_APP(V0);
float humidity;
float temperature;
int buttonblynk;
int ledapp;


void setup() 
{ 
  pinMode(button, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(rainsensor, INPUT);
  pinMode(motor, OUTPUT);
   pinMode(inout, OUTPUT);
  Serial.begin(115200);
  dht.begin(); 
  Blynk.begin(auth, ssid, pass,"blynk.cloud",8080);
  WiFi.mode(WIFI_STA);
  timer.run();
  Blynk.run();

}

BLYNK_WRITE(V3) 
{ 
  buttonblynk = param.asInt();
  if(buttonblynk)
   {
    onoff=!onoff;
    chieudc=!chieudc;
  } 
}

void nutnhan()
{
 if(digitalRead(button)==0)
  {
    while(digitalRead(button)==0){}
    onoff=!onoff;
    chieudc=!chieudc;
    Serial.println("nhan nut ");
  }
}
void ledonapp()
{ 
  countled=countled+1;
  delay(10);
  if(countled>=50)
  {
  if(chieudc!=ledapp && chieudc)
      {
        ledapp=1;
        LED_ON_APP.on();
      }
    if(chieudc!=ledapp && !chieudc)
      {
        ledapp=0;
      LED_ON_APP.off();
      }
      countled=0;
    }
}
void cambienmua()
{ 
  rainnext = digitalRead(rainsensor);
  if(rainnext && rainnext!=rainreg ) 
    {
      rainreg=rainnext;
      onoff = 1;
      chieudc=1;
    }
  rainreg=rainnext;
}
void cambiennhietdo()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if(temperature>35)
    {
      tempnext=true;
    }
  else
    {
      tempnext=false;
    }
  if(tempnext && tempnext!=tempreg ) 
    {
      tempreg=tempnext;
      onoff = 1;
      chieudc=1;
    }
  tempreg=tempnext;
}
void dkmotor(void)
{
  if(onoff)
  {
    if((chieudc)&&(digitalRead(switch2)))
        { 
        digitalWrite(motor,HIGH);
        digitalWrite(inout,HIGH);
        Serial.println("chay ra");
        }
    if((chieudc)&&(digitalRead(switch2)==0))
        {
        onoff=0;
        }
    if((!chieudc)&&(digitalRead(switch1)))
      { 
      digitalWrite(motor,HIGH);
      digitalWrite(inout,LOW);
      Serial.println("chay vao ");
      }
    if((!chieudc)&&(digitalRead(switch1)==0))
      {
      onoff=0;
      }
  }
  else
    {
    digitalWrite(motor,LOW);  
    Serial.println("dung");
    }
}

void blink ()
{
  count=count+1;
  delay(10);
  if(count>=50)
  {
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  count=0;
  Serial.println("gui blynk ");
  }
}
void loop() 
{
nutnhan();
ledonapp();
cambienmua();
blink ();
cambiennhietdo();
dkmotor();

/*Serial.println("Nhiet do: ");
Serial.println(temperature);
Serial.println("Do am: ");
Serial.println(humidity);*/
}