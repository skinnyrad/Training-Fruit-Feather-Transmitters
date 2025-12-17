#include <SPI.h>
#include <RH_RF69.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//WHAT DO YOU WANT THIS TO DO?
// 1 - Transmit a single, set frequency
// 2 - Burst; specify frequency, length of burst, and how often to activate
// 3 - Spread spectrum; specify start and stop freq and hop interval
// 4 - Spread Spectrum Burst - This option allows you to turn your spread spectrum signal
//                            off and on like a burst transmitter.
// 5 - Spread Spectrum, Frequency Hoping, Burst - Allows the user to do everything entailed in
//                                           option 4, but the signal will randomly jump
//                                           between two specified frequencies.

//Choose the number of the option you want.
int WhatToDo = 2;

//FOR OPTION 1 - Single Set Frequency
//Center frequency must be in MHz with a frequency between 809MHz AND 1023MHz
float centerFrequency = 915.0;

//FOR OPTION 2 - Burst
//The burst center frequency must be in MHz with a frequency between 809MHz AND 1023MHz
float burstCenterFrequency = 915.0;
//How long should the transmission occur? Time is in milliseconds: Ex: 5000 = 5 seconds
int burstLength = 500; 
//How often should this burst occur? Time is in milliseconds
int burstInterval = 2000;

//FOR OPTION 3 - Spread Spectrum
//The start frequency for the spread in MHz between 809MHz AND 1023MHz
float startFrequency = 915.0;
//The stop frequency for the spread in MHz between 809MHz AND 1023MHz
float stopFrequency = 918.0;
//How often should the signal jump to another frequency? Time in milliseconds. NOT ZERO!
int hopInterval = 1;

//FOR OPTION 4 - Spread Spectrum Burst
//Make sure all of the OPTION 3 variables are assigned and the burstLength and
//burstInterval variables from OPTION 2.

//FOR OPTION 5 - Spread Spectrum, Frequency Hoping, Burst (AKA the kitchen sink)
//Fill out hopInterval in OPTION 3. Fill out burstLength and burstInterval variables in
//OPTION 2. Also, fill out the variables below.

//How wide should the spread spectrum signal be? Bandwidth is in MHz.
float signalBandwidth = 2;

//As the spread spectrum signal jumps around, what is the upper most frequency it should
//bound itself by. The limit is 1023MHz. The number must be in MHz.
float upperLimit = 920.0;

//Define the lower limit as well
float lowerLimit = 910.0;

//SIGNAL AMPLITUDE
//Specify an amplitude level between 14(weakest) and 19(strongest).
int amplitude = 14;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// First 3 here are boards w/radio BUILT-IN. Boards using FeatherWing follow.
#if defined (__AVR_ATmega32U4__)  // Feather 32u4 w/Radio
  #define RFM69_CS    8
  #define RFM69_INT   7
  #define RFM69_RST   4

#elif defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)  // Feather M0 w/Radio
  #define RFM69_CS    8
  #define RFM69_INT   3
  #define RFM69_RST   4

#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_RFM)  // Feather RP2040 w/Radio
  #define RFM69_CS   16
  #define RFM69_INT  21
  #define RFM69_RST  17

#elif defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_CS    4  //
  #define RFM69_INT   3  //
  #define RFM69_RST   2  // "A"

#endif

//Test Data to be sent. This data can be many characters but it can affect
//transmission time.
uint8_t data[] = "0";

//Helps with timing delays without using Delay function
unsigned long startTime = 0;

//Helps with the selection of frequencies in OPTION 5
float randomFreq;

// Single instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup() 
{
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  while (!rf69.init()) 
  {
    while (1);
  }

  //Sets the power
  rf69.setTxPower(amplitude, true);
}

void loop()
{
  //The following if statements control what action the transmitter takes. The action
  //depends upon what the user has placed in the WhatToDo variable.
  
  //Option 1
  if (WhatToDo == 1)
  {
    //Sets the frequency to centerFrequency
    rf69.setFrequency(centerFrequency);
    
    //Sends the dadta
    rf69.send(data, sizeof(data));
    
    //Waits for data to be sent
    rf69.waitPacketSent();
  }
  
  //Option 2
  else if(WhatToDo == 2)
  {
    //Delays for the amount of time in burstInterval
    delay(burstInterval);
    rf69.setFrequency(burstCenterFrequency);
    
    //Stores how long the feather has been On. Number is in milliseconds.
    startTime = millis();
    
    //Continues to transmit for the length of time specified by burstLength
    while (millis()<=startTime+burstLength)
    {
      rf69.send(data, sizeof(data));
      rf69.waitPacketSent();
    }
  }
  
  //Option 3
  else if(WhatToDo == 3)
  {
    //Set the frequnecy to a random frequency between startFrequnecy and stopFrequency.
    //The math allows the random transmission to occur in tenth of a MHz increments.
    rf69.setFrequency((random(startFrequency*10,stopFrequency*10))/10.0);
    
    //Delay the transmission for time specified in hopInterval
    delay(hopInterval);
    rf69.send(data, sizeof(data));
    rf69.waitPacketSent();
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
      rf69.setFrequency((random(startFrequency*10,stopFrequency*10))/10.0);
      
      rf69.send(data, sizeof(data));
      rf69.waitPacketSent();
    }
  }

  //Options 5
  else if(WhatToDo == 5)
  {
    //Delay transmitting for the time specified in burstInterval
    delay (burstInterval);
    startTime = millis();

    //Calcutes a random frequency between the lower and upper limits specified
    randomFreq=(random(lowerLimit*10,upperLimit*10))/10.0;
    
    //Transmit for the amount of time specified in burstLength
    while(millis()<=startTime+burstLength)
    {
      //During the transmission time specified in burstLength, wait the amount of time
      //specified in hopInterval when switching frequencies.
      delay(hopInterval);

      //Sets the frequency using the bandwidth specified and the frequency limits
      rf69.setFrequency((random((randomFreq-(signalBandwidth/2))*10,(randomFreq+(signalBandwidth/2))*10))/10.0);
      
      rf69.send(data, sizeof(data));
      rf69.waitPacketSent();
    }
  }
}


