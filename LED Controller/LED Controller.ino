//The pin numbers of the 3 LEDs controlled by the switches
int led0 = 12;
int led1 = 11;
int led2 = 10;

//pin number of the on-board led
int onBoardLED = 13;

//The pin number of the led controlled by the photocell
int photocellLED = 5;

//The pin numbers of the two switches
int switchPin0 = 2;
int switchPin1 = 3;

//The state of each switch. LOW is off, HIGH is on
int switch0 = LOW;
int switch1 = LOW;

//The pin number of the photocell
int photocellPin = A1;

//Integer from 0 - 3 indicating which switches are on and off
int switchState = 0;

//State (HIGH or LOW) of each of the LEDs
int led0State = HIGH;
int led1State = LOW;
int led2State = LOW;
int led3State = LOW;
int onBoardLEDState = LOW;

//The value of the photocell input. Will be an integer between 0 and 1023
int photocellValue;

long previousMillisSwitches = 0;        // will store last time the switches were checked
long previousMillisLEDs = 0;            //will store the last time the leds were updated
long previousMillisOnBoardLED = 0;      //will store the last time the on board led blinked

long switchInterval = 200; //Interval at which to check the switches in milliseconds         
long ledInterval = 1000; //Interval at which to update LEDs in milliseconds
long onBoardLEDInterval = 2000; //Interval at witch to blink the on-board led in milliseconds

// the setup routine runs once when you press reset:
void setup() {                
  //Initializes the four pins connected to LEDs as outputs
  pinMode(led0, OUTPUT);    
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(photocellLED, OUTPUT);
  pinMode(onBoardLED, OUTPUT);

  //Initializes the two pins connected to switches as inputs
  pinMode(switchPin0, INPUT);
  pinMode(switchPin1, INPUT);
  
  //Sets the data rate for communication between the computer and the Arduino
  //In bits per secound(baud)
  Serial.begin(9600);

}

// the loop routine runs over and over again forever:
void loop() {  
  
  photocellValue = analogRead(photocellPin); //Reads the value from the photocell
  
  //If the photocell value is under 800 i.e. when the photocell is covered the LED will be turned on
  //Else it will be turned off
  if(photocellValue < 800){
    digitalWrite(photocellLED, HIGH);
  }else digitalWrite(photocellLED, LOW);  
  
  //Gets the current time in milliseconds
  unsigned long currentMillis = millis();
 
   //Reads the value, HIGH(on) or LOW(off) the switches
   switch0 = digitalRead(switchPin0);
   switch1 = digitalRead(switchPin1);

  //Checks if it's time to check the switches; that is, if the difference between
  //the current time and the last time the switches were checked is bigger than the 
  //interval. If so, the states of the switches are checked and the switchState is updated
  if(currentMillis - previousMillisSwitches > switchInterval) {
      // save the last time the switches were checked
     previousMillisSwitches = currentMillis; 

    if(switch0 == HIGH && switch1 == HIGH){//If both switches are on
        switchState = 3;        
    }
    if(switch0 == LOW && switch1 == HIGH){//If the first switch is off and the second on
        switchState = 2;
    }
    if(switch0 == HIGH && switch1 == LOW){//If the first switch is on and the second off
        switchState = 1;      
    }
    if(switch0 == LOW && switch1 == LOW){//If both switches are off
        switchState = 0;
    }
  }   
  
  //Blinks the on-board led on and off every 2 seconds
  if(currentMillis - previousMillisOnBoardLED > onBoardLEDInterval){
    previousMillisOnBoardLED = currentMillis; //saves the last time the led was updataed
    if(onBoardLEDState == HIGH){//If the led is on
      onBoardLEDState = LOW; //turn it off
    }else onBoardLEDState = HIGH; //turns the led on if it is off
  
    digitalWrite(onBoardLED, onBoardLEDState);  //writes the new led state to the pin
  }

  //Checks if it's time to update the leds; that is, if the difference between
  //the current time and the last time the leds were updated is bigger than the 
  //interval
  if(currentMillis - previousMillisLEDs > ledInterval){
     
     // save the last time the LEDs were updated
     previousMillisLEDs = currentMillis;  
    
     printStatus(); //Prints the status of leds, switches and photocell to the serial monitor
     
     //Depending on which led is currently on, and what state the switches are in, the next LED states
     //are selected     
     //If both switches are in the on position. The pattern is led0, led1, led2, led0
     if(switchState == 3){
       if(led0State == HIGH){
         led0State = LOW;
         led1State = HIGH;
         led2State = LOW;
       }else if(led1State == HIGH){
         led0State = LOW;
         led1State = LOW;
         led2State = HIGH; 
       }else if(led2State == HIGH){
         led0State = HIGH;
         led1State = LOW;
         led2State = LOW;
       }else{
         led0State = HIGH;
         led1State = LOW;
         led2State = LOW;
       }
     }
     
     //If the first switch is off and the second is on. The pattern is led0, led2, led1, led0
     if(switchState == 2){
       if(led0State == HIGH){
         led0State = LOW;
         led1State = LOW;
         led2State = HIGH;
       }else if(led1State == HIGH){
         led0State = HIGH;
         led1State = LOW;
         led2State = LOW;
       }else if(led2State == HIGH){
         led0State = LOW;
         led1State = HIGH;
         led2State = LOW;
       }else{
         led0State = HIGH;
         led1State = LOW;
         led2State = LOW;
       }
     }
     
     //If the first switch is on and the second is off. All three leds blink on and off
     if(switchState == 1){ 
       if(led0State == HIGH || led1State == HIGH || led2State == HIGH){
         led0State = LOW;
         led1State = LOW;
         led2State = LOW;
       }else{
         led0State = HIGH;
         led1State = HIGH;
         led2State = HIGH;
       }
     }
     
     else{
       //Both switches off, so do nothing keeping current led status
     }
     
    //Updates the status of the LEDs. 
    digitalWrite(led0, led0State);
    digitalWrite(led1, led1State);
    digitalWrite(led2, led2State);     
  } 

}
  
  //Displays the status of the switches, LEDs, and photocell value on the serial monitor
  void printStatus(){
    Serial.print("SWITCHES: ");
    if(switch0 == HIGH){
      Serial.print("ON ");
    }else Serial.print("OFF ");
    
    if(switch1 == HIGH){
      Serial.print("ON ");
    }else Serial.print("OFF");
    
    Serial.print("LEDS: ");
    
    if(led0State == HIGH){
      Serial.print("ON ");    
    }else Serial.print("OFF ");
    
    if(led1State == HIGH){
      Serial.print("ON ");    
    }else Serial.print("OFF ");
    
    if(led2State == HIGH){
      Serial.print("ON ");    
    }else Serial.print("OFF ");
    
    if(onBoardLEDState == HIGH){
      Serial.print("ON ");    
    }else Serial.print("OFF ");
    
   Serial.print("Photocell Value: ");
   Serial.print(photocellValue, DEC);
   
   Serial.print(" Photocell LED: ");
       if(digitalRead(photocellLED) == HIGH){
      Serial.print("ON ");    
    }else Serial.print("OFF "); 
    
    Serial.println();
  
  }



