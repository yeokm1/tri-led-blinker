#include "LowPower.h"
#define NUM_LETTERS 3

//Uncomment this if you want to skip the reverse states
//#define REDUCE_FUNCTION

#define PIN_L1 PB0
#define PIN_L2 PB1
#define PIN_L3 PB2
#define PIN_BUTTON PB3

//Accept only one button input every 0.3s
#define DEBOUNCE_TIME 300 //0.3s

#define DELAY_BETWEEN_LETTERS 750


typedef enum {
  STATE_FORWARD, STATE_REVERSE, STATE_BLINK_TWICE_FORWARD, STATE_BLINK_TWICE_BACKWARD, STATE_BLINK_ALL} 
BLINK_STATE ;
volatile BLINK_STATE blinkState = STATE_FORWARD;


const int PINS[NUM_LETTERS] = {PIN_L1, PIN_L2, PIN_L3};


int currentPinToLight = 0;

bool allCurrentlyOn = false;


//Initialise the last time the button is pressed
long buttonLastPressedTime = millis();

long timeLastBlink = millis();


int timesBlinkedAtOneSpot = 0;

void setup() {

  pinMode(PIN_BUTTON, INPUT);
  
  for(int currentPos = 0; currentPos < NUM_LETTERS; currentPos++){
    pinMode(PINS[currentPos], OUTPUT);
  }

}

void loop() {

  if(shouldProceedToNextBlink()){
  
    switch(blinkState){
      case STATE_FORWARD: blinkForward();
      break;
      case STATE_REVERSE: blinkReverse();
      break;
      case STATE_BLINK_TWICE_FORWARD: blinkTwiceForward();
      break;
      case STATE_BLINK_TWICE_BACKWARD: blinkTwiceBackward();
      break;
      case STATE_BLINK_ALL: blinkAll();
      break;
      default: blinkForward();
    }
   }
  
  
  if(isButtonPressed()){
    buttonPressed();
  }


  delay(10);

  LowPower.powerSave(SLEEP_60MS, ADC_OFF, BOD_OFF, TIMER2_ON); 
}


bool isButtonPressed(){
  int buttonState = digitalRead(PIN_BUTTON);
  long currentTime = millis();
   
  //This code is to debounce the button input.
  if(buttonState == LOW && ((currentTime - buttonLastPressedTime) > DEBOUNCE_TIME)){
    buttonLastPressedTime = currentTime;
    return true;
  } else {
    return false;
  }
}

void blinkForward(){

  if(currentPinToLight >= NUM_LETTERS){
    currentPinToLight = 0;
  }
   
  onlyTurnOnThisLED(currentPinToLight);
   
  currentPinToLight++;

}



void blinkReverse(){

  if(currentPinToLight < 0){
    currentPinToLight = NUM_LETTERS - 1;
  }
   
  onlyTurnOnThisLED(currentPinToLight);
   
  currentPinToLight--;
}


void blinkTwiceForward(){
  if(currentPinToLight >= NUM_LETTERS){
    currentPinToLight = 0;
  }

  if(timesBlinkedAtOneSpot == 0){
      onlyTurnOnThisLED(currentPinToLight);
      timesBlinkedAtOneSpot++;
  } else if(timesBlinkedAtOneSpot == 1){
      writeToAllLeds(false);
      timesBlinkedAtOneSpot++;
  } else if(timesBlinkedAtOneSpot == 2){
      onlyTurnOnThisLED(currentPinToLight);
      timesBlinkedAtOneSpot++;
  } else {
    writeToAllLeds(false);
    timesBlinkedAtOneSpot = 0;
    currentPinToLight++;
  }
}

void blinkTwiceBackward(){
  if(currentPinToLight < 0){
    currentPinToLight = NUM_LETTERS - 1;
  }

  if(timesBlinkedAtOneSpot == 0){
      onlyTurnOnThisLED(currentPinToLight);
      timesBlinkedAtOneSpot++;
  } else if(timesBlinkedAtOneSpot == 1){
      writeToAllLeds(false);
      timesBlinkedAtOneSpot++;
  } else if(timesBlinkedAtOneSpot == 2){
      onlyTurnOnThisLED(currentPinToLight);
      timesBlinkedAtOneSpot++;
  } else {
    writeToAllLeds(false);
    timesBlinkedAtOneSpot = 0;
    currentPinToLight--;
  }
}

void blinkAll(){
  
  allCurrentlyOn = !allCurrentlyOn;

  int value = allCurrentlyOn ? HIGH : LOW;

  writeToAllLeds(value);
  
}

bool shouldProceedToNextBlink(){
  long currentTime = millis();
  

  if((currentTime - timeLastBlink) > DELAY_BETWEEN_LETTERS){
    timeLastBlink = currentTime;
    return true;
  }

  return false;
}

#ifdef REDUCE_FUNCTION

void buttonPressed(){
  switch(blinkState){
    case STATE_FORWARD: blinkState = STATE_BLINK_TWICE_FORWARD;
    break;
    case STATE_BLINK_TWICE_FORWARD: blinkState = STATE_BLINK_ALL;
    break;
    case STATE_BLINK_ALL : blinkState = STATE_FORWARD;
    break;
    default: blinkState = STATE_FORWARD;
  }
}

#else

void buttonPressed(){
  switch(blinkState){
    case STATE_FORWARD: blinkState = STATE_REVERSE;
    break;
    case STATE_REVERSE: blinkState = STATE_BLINK_TWICE_FORWARD;
    break;
    case STATE_BLINK_TWICE_FORWARD: blinkState = STATE_BLINK_TWICE_BACKWARD;
    break;
    case STATE_BLINK_TWICE_BACKWARD: blinkState = STATE_BLINK_ALL;
    break;
    case STATE_BLINK_ALL : blinkState = STATE_FORWARD;
    break;
    default: blinkState = STATE_FORWARD;
  }
}

#endif




void writeToAllLeds(int state){
  for(int currentPos = 0; currentPos < NUM_LETTERS; currentPos++){
    digitalWrite(PINS[currentPos], state);
  }
}

void onlyTurnOnThisLED(int pinPositionNumber){

  for(int currentPos = 0; currentPos < NUM_LETTERS; currentPos++){

    if(currentPos == pinPositionNumber){
      digitalWrite(PINS[currentPos], HIGH);
    } else {
      digitalWrite(PINS[currentPos], LOW);
    }  
  }
}



