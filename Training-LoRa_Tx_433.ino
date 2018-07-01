#include <SPI.h>
#include <RH_RF95.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//WHAT DO YOU WANT THIS TO DO?
// 1 - Transmit a single, set frequency
// 2 - Burst; specify frequency, length of burst, and how often to activate
// 3 - Spread spectrum; specify start and stop freq and hop interval
// 4 - Spread Spectrum Burst - This option allows you to turn your spread spectrum signal
//                            off and on like a burst transmitter.

//Choose the number of the option you want.
int WhatToDo = 4;

//FOR OPTION 1 - Single Set Frequency
//Center frequency must be in MHz with a frequency between 363MHz AND 569MHz
float centerFrequency = 433.0;

//FOR OPTION 2 - Burst
//The burst center frequency must be in MHz with a frequency between 363MHz AND 569MHz
float burstCenterFrequency = 433.0;
//How long should the transmission occur? Time is in milliseconds: Ex: 5000 = 5 seconds
int burstLength = 500; 
//How often should this burst occur? Time is in milliseconds
int burstInterval = 2000;

//FOR OPTION 3 - Spread Spectrum
//The start frequency for the spread in MHz between 363MHz AND 569MHz
float startFrequency = 431.0;
//The stop frequency for the spread in MHz between 363MHz AND 569MHz
float stopFrequency = 437.0;
//How often should the signal jump to another frequency? Time in milliseconds
int hopInterval = 0;

//FOR OPTION 4 - Spread Spectrum Burst
//Make sure all of the OPTION 3 variables are assigned and the burstLength and
//burstInterval variables from OPTION 2.

//SIGNAL AMPLITUDE
//Specify an amplitude level between 5(weakest) and 23(strongest). This transmitter
//is a beast. Even 5 will blow you away.
int amplitude = 5;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Defines pins needed for RF Tx & Rx to function
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7 

//Test Data to be sent. This data can be many characters but it can affect
//transmission time.
uint8_t data[] = "0";

//Helps with timing delays without using Delay function
unsigned long startTime = 0;

// Single instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) 
  {
    while (1);
  }

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(amplitude, false);
}

void loop()
{
  //The following if statements control what action the transmitter takes. The action
  //depends upon what the user has placed in the WhatToDo variable.
  
  //Option 1
  if (WhatToDo == 1)
  {
    //Sets the frequency to centerFrequency
    rf95.setFrequency(centerFrequency);
    
    //Sends the dadta
    rf95.send(data, sizeof(data));
    
    //Waits for data to be sent
    rf95.waitPacketSent();
  }
  
  //Option 2
  else if(WhatToDo == 2)
  {
    //Delays for the amount of time in burstInterval
    delay(burstInterval);
    rf95.setFrequency(burstCenterFrequency);
    
    //Stores how long the feather has been On. Number is in milliseconds.
    startTime = millis();
    
    //Continues to transmit for the length of time specified by burstLength
    while (millis()<=startTime+burstLength)
    {
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
  }
  
  //Option 3
  else if(WhatToDo == 3)
  {
    //Set the frequnecy to a random frequency between startFrequnecy and stopFrequency.
    //The math allows the random transmission to occur in tenth of a MHz increments.
    rf95.setFrequency((random(startFrequency*10,stopFrequency*10))/10.0);
    
    //Delay the transmission for time specified in hopInterval
    delay(hopInterval);
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
  }
  
  //Options 4
  else if(WhatToDo == 4)
  {
    //Delay transmitting for the time specified in burstInterval
    delay (burstInterval);
    startTime = millis();

    //Transmit for the amount of time specified in burstLength
    while(millis()<=startTime+burstLength)
    {
      //During the transmission time specified in burstLength, wait the amount of time
      //specified in hopInterval when switching frequencies.
      delay(hopInterval);
      
      //As in option 3, causes the spectrum spread to be random.
      rf95.setFrequency((random(startFrequency*10,stopFrequency*10))/10.0);
      
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
  }
}

