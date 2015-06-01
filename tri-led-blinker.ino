#define NUM_LETTERS 3

#define PIN_W1 PB0
#define PIN_J2 PB1
#define PIN_S3 PB2


const int PINS[NUM_LETTERS] = {PIN_W1, PIN_J2, PIN_S3};

int currentPinToLight = 0;

void setup() {
  
  for(int currentPos = 0; currentPos < NUM_LETTERS; currentPos++){
    pinMode(PINS[currentPos], OUTPUT);
  }

}

void loop() {
  if(currentPinToLight >= NUM_LETTERS){
    currentPinToLight = 0;
  }
   
  onlyTurnOnThisLED(currentPinToLight);
   
  currentPinToLight++;
  
  customDelayBetweenLetters();

  customDelayBetweenLetters();
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

void customDelayBetweenLetters(){
  delay(750);

}
