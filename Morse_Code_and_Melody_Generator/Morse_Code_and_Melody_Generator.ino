int dot = 200; //Duration in milliseconds of a dot
int dash = dot*3; //Duration in milliseconds of a dash
int space = dot*7; //Duration in milliseconds of a space

//Pins of the seven segment display
int sseg0 = 13;
int sseg1 = 12;
int sseg2 = 11;
int sseg3 = 10;
int sseg4 = 9;
int sseg5 = 8;
int sseg6 = 7;

int buzzer = 3; //Buzzer pin
int LED = 5;//Led pin

boolean isModeSelected = false; //Whether or note a mode has been selected
int mode; //current mode, morse or melody
const int MORSE_MODE = 0;
const int MELODY_MODE = 1;

//An array containing the morse represention of a - z and 0 - 9
char* morseCodes[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",".---", "-.-", ".-..", 
                      "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", 
                      "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
                      "---..", "----.", ":" };

//An array containing info on which segments of the SSD should be on or off. a - z and 0 - 9
int ssd[][7] = {{1,1,1,0,1,1,1}, {0,0,1,1,1,1,1}, {1,0,0,1,1,0,1}, {0,1,1,1,1,1,0}, {1,0,0,1,1,1,1}, {1,0,0,0,1,1,1},
                {1,1,1,1,0,1,1}, {0,1,1,0,1,1,1}, {0,0,0,0,1,0,1}, {0,1,1,1,1,0,0}, {0,1,1,0,1,1,1}, {0,0,0,1,1,0,1},
                {1,0,1,0,1,0,0}, {0,0,1,0,1,1,0}, {1,1,1,1,1,0,1}, {1,1,0,0,1,1,1}, {1,1,1,0,0,1,1}, {0,0,0,0,1,1,0}, 
                {1,0,1,1,0,1,1}, {0,0,0,1,1,1,1}, {0,1,1,1,1,0,1}, {0,0,1,1,1,0,0}, {0,1,0,1,0,0,1}, {0,1,1,0,1,1,1},
                {0,1,1,1,0,1,1}, {1,1,0,1,1,1,0}, {1,1,1,1,1,0,1}, {0,1,1,0,0,0,0}, {1,1,0,1,1,1,0}, {1,1,1,1,0,1,0}, 
                {0,1,1,0,1,1,0}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}, 
                {0,0,0,0,0,0,0}};
           
//Frequency in Hz of the notes A4 - G4(ish - maybe)in that order     
int notes[] = {440, 494, 262, 294, 330, 349, 392, 0}; 

void setup() {
  //Initiates the modes of all output pins
  pinMode(sseg0, OUTPUT);
  pinMode(sseg1, OUTPUT);
  pinMode(sseg2, OUTPUT);
  pinMode(sseg3, OUTPUT);
  pinMode(sseg4, OUTPUT);
  pinMode(sseg5, OUTPUT);
  pinMode(sseg6, OUTPUT);
  pinMode(LED, OUTPUT);  
  pinMode(buzzer, OUTPUT);
  
  //Sets the data rate for communication between the computer and the Arduino
  //In bits per secound(baud)
  Serial.begin(9600);
  
  //Initializes the Seven segment display as all off.
  //When HIGH is written to the segment, it turns off, when low it turns on  
  clearSSD();  
  Serial.println("Enter 1 for Morse, 2 For melody");
  
}

void clearSSD(){
  //Writes HIGH to each seven segment display to turn them all off
  digitalWrite(sseg0, HIGH);
  digitalWrite(sseg1, HIGH);
  digitalWrite(sseg2, HIGH);  
  digitalWrite(sseg3, HIGH);
  digitalWrite(sseg4, HIGH);
  digitalWrite(sseg5, HIGH);
  digitalWrite(sseg6, HIGH);

}

void loop(){
  
  clearSSD();//Clears the SSD when waiting for user input
    
  if(isModeSelected == false){//If a mode has not been selected, the user will be prompted to enter 1(morse) or 2(melody)
  
    char selection;//character inputed by the user
    if(Serial.available() > 0){
      selection = Serial.read();//reads a character from the serial monitor
      //Morse Mode
      if(selection == '1'){
        mode = MORSE_MODE;
        isModeSelected = true;
        Serial.println("Morse Mode Selected. Enter ! to quit");
      }else if(selection == '2'){//Melody Mode
        mode = MELODY_MODE;
        isModeSelected = true;
        Serial.println("Melody Mode Selected. Enter ! to quit");
      }else{
        Serial.println("Invalid Selection, try again");//Invailid (not 1 or 2) character inputed
      }
    }
  }else if(isModeSelected == true && mode == MORSE_MODE){ //If the current mode is Morse code mode
  
    char info;
    if(Serial.available() > 0){
  
      info = Serial.read();
      
      //Checks to see if the inputted character is upper case, lower case or a number
      //A constant is subtracted from the ACSII value of the character to get the index
      //of the morse representation in the morse array
      if(info >= 65 && info <= 90){ //Upper case
        outputMorse(info - 65);
      }else if(info >= 97 && info <= 122){//Lower case
        outputMorse(info - 97);
      }else if(info >= 48 && info <=  57){//Number
        outputMorse(info - 22);
      }else if(info == 32){//space
        outputMorse(36);  
      }else if(info == '!'){//When an ! is inputted, the current mode is quit
        isModeSelected = false;
        Serial.println("Enter 1 for Morse, 2 For melody");
      } 
    }
  
  }else if(isModeSelected == true && mode == MELODY_MODE){//If the current mode is melody mode
    char note; //a character from a - g representing a musical note
    if(Serial.available() > 0){
      note = Serial.read();
      
      if(note == '!'){//If an ! is inputted, the current mode is quit
        isModeSelected = false;
        Serial.println("Enter 1 for Morse, 2 For melody");
      }else{
        playNote(note); //Plays the inputted note on the buzzer   
      }      
    }  
  }

}

//Displays the current character, or note on the ssd
//param i is the index of the character in the ssd array
void displaySSD(int i){
  //iterates through each element(on or off) of the array corresponding to the character
 //and turns the specified segment on or off 
  for(int n = 0; n < 7; n++){
    if(ssd[i][n] == 1) digitalWrite(13-n, LOW); //If the segment should be ON
    else digitalWrite(13 -n, HIGH);  //If the segment should be OFF
  }
}

//Out puts the morse code corresponding to the index
void outputMorse(int index){
  char *morse = morseCodes[index]; //String containing the morse representation 
  displaySSD(index); //Displays the character on the SSD  
  
  //For each character in the morse string, output the series of dots and dashes to the buzzer and LED
  for(int i = 0; i < strlen(morse); i++){
    digitalWrite(LED, HIGH);//turns the LED on
    analogWrite(buzzer, 250);//turns the buzzer on
    
    //The delay depends on whether a dot or dash is to be played
  
    if(morse[i] == '.')
      delay(dot);
    else if(morse[i] == '-')
      delay(dash);
    else{//If the character is a space, the buzzer and led are turned off
      digitalWrite(LED, LOW);
      analogWrite(buzzer, 0);
      delay(space);
    }
    
    //Turns the led and buzzer off
    digitalWrite(LED, LOW);
    analogWrite(buzzer, 0);
    //delays for a dot between characters
    delay(dot);
  }
  
}

//Plays the specified note
void playNote(char note){
  int noteIndex;//index of the note in the notes array
  int noteLength;//length of time the note is to be played for
  
  if(note >= 65 && note <= 71){//If an upper case, note is played for 1 second
    noteIndex = note - 65;
    noteLength = 1000;
    tone(buzzer, notes[noteIndex], noteLength);//Plays the note on the buzzer
  }else if(note >= 97 && note <= 103){//if a lower case, note is played for 1/2 second
    noteIndex = note - 97;
    noteLength = 500;
    tone(buzzer, notes[noteIndex], noteLength);//plays the note on the buzzer
  }else if(note == 32){
    
  }  
  displaySSD(noteIndex);//Displays the note being played on the SSD

  delay(noteLength);
  
}
