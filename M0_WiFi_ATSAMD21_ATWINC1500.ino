#include <SPI.h>
#include <WiFi101.h>

//***********************************************************************************
//WHAT DO YOU WANT THIS TO DO?
// 1 - The WiFi client will probe request based on the interval delayTime
// 2 - The WiFi client will transmit randomly between 2 specified times

//Choose the number of the option you want.
int WhatToDo = 2;

//FOR OPTION 1
//Chose how often, in milliseconds, to transmit a WiFi signal
int delayTime = 10000; 

//FOR OPTION 2
//Give the lower and upper limits, in milliseconds, of how often to transmit a signal
int lowerLimit = 5000;
int upperLimit = 300000; //5 minutes (300000) is a reasonable upper limit although
                         //20 minutes (1200000) is not unheard of.
//***********************************************************************************

int ledPin = 13;
void setup() 
{
  WiFi.setPins(8,7,4,2);
  pinMode (ledPin, OUTPUT);
}

void loop() 
{
  if(WhatToDo == 1) //OPTION 1
  {
    delay(delayTime);
    WiFi.scanNetworks();
  }

  if(WhatToDo == 2)//OPTION 2
  {
    delay(random(lowerLimit,upperLimit));
    WiFi.scanNetworks();
    
    //Blinks whenever randomly timed WiFi signal transmits
    digitalWrite(ledPin,HIGH);
    delay(250);
    digitalWrite(ledPin,LOW);
    delay(250);
    digitalWrite(ledPin,HIGH);
    delay(250);
    digitalWrite(ledPin,LOW);
  }
}

