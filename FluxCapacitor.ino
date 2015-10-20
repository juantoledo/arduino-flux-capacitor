
#include <SoftPWM.h>

//Prototype for optional "wave lights effect" feature
void turnMainLights(int turnSpeed, int intensity, int optionalDelay, boolean wave=false);

/*Y form Leds arrays for 'time travel' preset*/
int mainLights[][4] = {{5, 2, 11},
                    {6,3, 10},
                    {7,4, 9},
                    {8,0,0}};
int cornerLights[] = {18, 19, 16, 17};

/*clock form Leds arrays for 'clock' preset*/
int clockLights[][4] = {{8,4,3,2},
                        {8,9,10,11},
                        {8,7,6,5}};


const int buttonPin = 12;
int buttonState = 0;  // variable for reading the pushbutton status

void setup()
{
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  SoftPWMBegin();
  initializeLights();
}

void initializeLights()
{
  for(int i=0;i<4;i++)
  {
     SoftPWMSetFadeTime(cornerLights[i], 4000, 4000);
     //SoftPWMSetPercent(cornerLights[i], 0); 
  }
  for(int i=0;i<4;i++)
  {
    for(int k=0;k<=2;k++)
    {
      SoftPWMSetFadeTime(mainLights[i][k], 4000, 4000);
      //SoftPWMSetPercent(cornerLights[i], 0); 
    }
  }
}

/*time travel global timers*/
int timer = 150;
int ttimer = 200;
void timeTravel()
{ 
  turnMainLights(timer, 100, ttimer, true);
  turnMainLights(timer, 0, ttimer, true);
  
  if(ttimer > 50)
  {
    ttimer = ttimer - 5 ;
    timer = timer - 5;   
  }
  else if(ttimer <= 50 && ttimer > 0)
  {
    //turn on all Corner Lights, DOC !!!!
    turnCornerLights(5000,100,10);
    
    ttimer = ttimer - 1 ;
    timer = timer - 1;   
  }
  else if(ttimer <= 15 && ttimer > 0)
  {
    ttimer = ttimer - 5 ;
    timer = timer - 5;  
  }
  //88 MPH reached, TIME TRAVEL !!!! Three explosions and turn off all Corner Lights
  if(ttimer <= 0)
  {
    turnMainLights(0, 100, -1);
    delay(400);
    turnMainLights(500, 0, -1);
    delay(800);
    turnMainLights(0, 100, -1);
    delay(400);
    turnMainLights(500, 0, -1);
    delay(800);
    turnMainLights(0, 100, -1);
    delay(400);
    turnCornerLights(2000,0,-1);
    turnMainLights(1500, 0, -1);
    timer = 150;
    ttimer = 200;
    delay(4000);
  }
}

/*global clock timer & hit*/
int clockDelay = 1000;
int clockHit = 1000;
void clock(boolean roulette)
{
  for(int i=0;i<3;i++)
  {
    
    for(int k=0;k<=3;k++)
    {
      turnSpecificLight(clockLights[i][k], 0, 100);
    }
    if(roulette)
    {
      turnCornerLights(0, 100, -1);
      clockDelay = clockDelay - 5;
      if(clockDelay<20)
      {
        clockDelay = 20;
        for(int i=0 ; (i<clockHit && clockDelay<=20) ; i++)
        {
          if(i >= clockHit - 1)
          {
            clockDelay = 1000;
          }
        }
      }
    }
    else{
      turnCornerLights(0, 0, -1);
      clockDelay = 1000; 
    }
    delay(clockDelay);
    for(int k=0;k<=3;k++)
    {
      turnSpecificLight(clockLights[i][k], 0, 0);
    }
    
  } 
  turnCornerLights(0, 0, -1);
}

int preset = 1;
void loop()
{
   // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if(buttonState == 1)
  {
      preset++;
      if(preset > 3) preset = 1;
  }
  switch(preset)
  {
    case 1: timeTravel(); break;
    case 2: clock(true); break;
    case 3: clock(false); break;
  }
}


void turnSpecificLight(int lightPin, int turnSpeed, int intensity) 
{
     SoftPWMSetFadeTime(lightPin, turnSpeed, turnSpeed);
     SoftPWMSetPercent(lightPin, intensity);   
}


void turnCornerLights(int turnSpeed, int intensity, int optionalDelay)
{
  for(int i=0;i<4;i++)
  {
    turnSpecificLight(cornerLights[i], 2000, intensity);
    if(optionalDelay != -1)
    {
      delay(optionalDelay); 
    }
  }
  
}

void turnMainLights(int turnSpeed, int intensity, int optionalDelay, boolean wave)
{ 
  for(int i=0;i<4;i++)
  {
    for(int k=0;k<=2;k++)
    {
      turnSpecificLight(mainLights[i][k], turnSpeed, intensity);
    }
    if(optionalDelay != -1)
    {
       delay(optionalDelay); 
    }
    //wave is turning off the precedent light
    if(wave)
    {
      for(int j=0;j<=2;j++)
      {
        turnSpecificLight(mainLights[i][j], turnSpeed + 20, 0);
      }
    }
  }
}

void turnMainOnOff(int delayBetweenHops, int turnDelay, int intensityOn, int intensityOff, int hops)
{
 for(int i=0;i<hops;i++)
 { 
  turnMainLights(turnDelay, intensityOff, -1);
  delay(delayBetweenHops);
  turnMainLights(turnDelay, intensityOn, -1);
  delay(delayBetweenHops);
 }
}
