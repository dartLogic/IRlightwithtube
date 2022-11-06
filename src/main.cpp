/*
IR sensor: HS0038A2
Remote type: Redmi note 4
app : MI remote version 5.8.5.8
device: Philips TV
Setup steps: TV -> Philips -> (is tv on ?) off-> (checking available configuration
1/9, Power) -> (does device turn of/off ? ) yes

Ir codes 
Vol+ = 0x10, 0x810   //LED Increase intensity 
Vol- = 0x11, 0x811   // LED Decrease intensity 
Ch+ = 0x20, 0x820    //LED MAX
Ch- = 0x21, 0x821    //LED OFF
ok = 0xDD53082F, 0xB411F6DE // Tube On
power = 0xC, 0x80C          // Tube off
*/

#include <Arduino.h>    //remove this line if using official Arduino IDE
#include <IRremote.h>

#define VolPlus 0x10            // LED Increase intensity 
#define VolPlus2 0x810          // LED Increase intensity 
#define VolPlus3 0x45B          // LED Increase intensity 
#define VolPlus4 0x1045B          // LED Increase intensity 
#define VolMinus 0x11           // LED Decrease intensity
#define VolMinus2 0x811         // LED Decrease intensity 
#define VolMinus3 0x45A        // LED Decrease intensity 
#define VolMinus4 0x1045A         // LED Decrease intensity 
#define ChannelPlus 0x20        // LED MAX
#define ChannelPlus2 0x820      // LED MAX
#define ChannelPlus3 0x458     // LED MAX
#define ChannelPlus4 0x10458     // LED MAX
#define ChannelMinus 0x21       // LED OFF
#define ChannelMinus2 0x821     // LED OFF
#define ChannelMinus3 0x459     // LED OFF
#define ChannelMinus4 0x10459     // LED OFF
#define Ok 0xDD53082F           // Tube On
#define Ok2 0xB411F6DE          // Tube On
#define Ok3 0x45C           // Tube On
#define Ok4 0x1045C          // Tube On
#define Power 0xC               // Tube off
#define Power2 0x80C           // Tube off
#define Power3 0x431            // Tube off
#define Power4 0x10431            // Tube off


const int RECV_PIN = 3;       //OUT of HS0038A2
const int manyLedPin = 5;     //to gate of MOSFET
const int tubePin = 6;        // to control relay of tubelight
const int singleLedPin = 9;      //single LED for very small brightness
const int maxIntensity = 26;
const int manyLedThreshold = 13;    //after this intensity manyleds will start

void singleLedWrite();
void manyLedWrite();

IRrecv irrecv(RECV_PIN);
decode_results results;
int intensity = 0;
bool tubeStatus = true;

void setup()
{
  //Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(tubePin, OUTPUT);
  digitalWrite(tubePin, LOW);
}

void loop()
{
  delay(100);
  if (irrecv.decode(&results))
  {
    //Serial.println(results.value, HEX);
    switch(results.value)
  {
    case Power:
    case Power2:
    case Power3:
    case Power4:  
    digitalWrite(tubePin, HIGH);
    if (tubeStatus == true)
    {
      intensity = maxIntensity;
    }
    tubeStatus = false;     
    break;    

    case Ok:
    case Ok2:
    case Ok3:
    case Ok4:
    digitalWrite(tubePin, LOW);    
    tubeStatus = true;
    intensity = 0;
    break;

    case VolMinus:    
    case VolMinus2:
    case VolMinus3:
    case VolMinus4:
    intensity = intensity -1;
    if(intensity<0)
    {
         intensity=0;
    }
    break;

    case VolPlus:
    case VolPlus2:
    case VolPlus3:
    case VolPlus4:
    intensity = intensity +1;
    if(intensity>maxIntensity)
    {
      intensity=maxIntensity;
    }
    break;

    case ChannelPlus:
    case ChannelPlus2:
    case ChannelPlus3:
    case ChannelPlus4:

    intensity = maxIntensity;
    break;

    case ChannelMinus:
    case ChannelMinus2:
    case ChannelMinus3:
    case ChannelMinus4:
    intensity = 0;
    break;
  }  
    irrecv.resume();    
  }  
  singleLedWrite();
  manyLedWrite();     
}

int fibonacci (int);
void singleLedWrite()
{  
  int ledIntensity = constrain (intensity, 0, manyLedThreshold);
  
  int brightness = fibonacci(ledIntensity);                //brithness will be nth terms of fibonacci series
  if(tubeStatus==true)
  {
    brightness = 0;
  }
  analogWrite(singleLedPin, brightness);  
}

void manyLedWrite()
{  
  int manyledIntensity = intensity -manyLedThreshold;
  if(manyledIntensity < 0)
  {
    manyledIntensity = 0;
  }
  int brightness = map(manyledIntensity, 0, (maxIntensity-manyLedThreshold), 0, 255);
  if(tubeStatus==true)
  {
    brightness = 0;
  }  
  analogWrite(manyLedPin, brightness);  
}

int fibonacci(int n)
{
   if (n <= 1)
      return n;
   return fibonacci(n-1) + fibonacci(n-2);
}