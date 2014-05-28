//LCD SETUP
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//CHARACTER BYTES
byte canon[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00011,
  0b11111,
  0b00011,
  0b00001,
  0b00000
};

byte laser[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte poof[8] = {
  0b10101,
  0b01110,
  0b00100,
  0b11000,
  0b00011,
  0b00100,
  0b01110,
  0b10101
};

byte invaderZero[8] = {
  0b00110,
  0b01000,
  0b11111,
  0b01110,
  0b01110,
  0b11111,
  0b01000,
  0b00110
};

byte invaderOne[8] = {
  0b11000,
  0b00100,
  0b11111,
  0b01110,
  0b01110,
  0b11111,
  0b00100,
  0b11000
};

byte invaderTwo[8] = {
  0b00001,
  0b00010,
  0b01101,
  0b11110,
  0b11110,
  0b01101,
  0b00010,
  0b00001
};

//INVADERS
const int invader[] = {4, 5, 6};  //these will be the 3 invaders (see CHARACTERS)
int number = 0;                   //number determines wich invader is used 0,1 or 2
int directionInvader = 0;         //0 = left, 1 = right

//COLUMNS & LINES
int columnInvader = 0;
int lineInvader = 0;
int lineCanon = 0;
int columnLaser = 19;             //the laser begins at the last column
int lineLaser = 0;

//OTHER INTS
int counter = 0;                  //count how many times the void loop has been completed
int level = 0;                    //how many invaders you have hit
int difficulty = 3;               //the lower this number, the faster the invaders will go

//BUTTONS
const int left = A5;
const int right = A4;

void setup() {
  
//CHARACTERS  
 lcd.createChar(1, canon);
 lcd.createChar(2, laser);
 lcd.createChar(3, poof);
 lcd.createChar(4, invaderZero);
 lcd.createChar(5, invaderOne);
 lcd.createChar(6, invaderTwo);
 
//STARTING LCD
 lcd.begin(20, 4); 
}

void loop() {
  
//GOING LEFT/RIGHT
  int leftValue = analogRead(left);        //I used analog inputs to make it easier to replace the buttons with something like a potmeter for going left/right
  int rightValue = analogRead(right);
  if(leftValue > 511 && rightValue < 511 && lineCanon != 0){      //left (left button pressed)
    --lineCanon;
  }
  if(leftValue < 511 && rightValue > 511 && lineCanon != 3){      //right (right button pressed)
    ++lineCanon;
  }
  
//SHOOTING  
  if(leftValue > 511 && rightValue > 511 && columnLaser == 19){ //shooting (both buttons pressed)
   lineLaser = lineCanon;                 //set laser on the correct line
  }
  if((leftValue > 511 && rightValue > 511) || columnLaser != 19){
    --columnLaser;
    lcd.setCursor(columnLaser, lineLaser);
    lcd.write(2);                          //move and write laser
  }
  if(columnLaser == 0){
   columnLaser = 19;                       //reset laser
  }
  
//ANIMATION  
  lcd.setCursor(19, lineCanon);
  lcd.write(1);                          //write canon

  lcd.setCursor(columnInvader, lineInvader);
  lcd.write(invader[number]);            //write invader
  
  delay(200);
  lcd.clear();                           //delay & clear
  
  if(counter == difficulty){
    counter = 0;                         //reset counter
    
    if(directionInvader == 0 && lineInvader == 3){
     directionInvader = 1;
     --lineInvader;
     ++columnInvader;                    //move invader down & change direction when invader is all the way at the left 
    }
    else if(directionInvader == 1 && lineInvader == 0){
     directionInvader = 0;
     ++lineInvader; 
     ++columnInvader;                    //move invader down & change direction when invader is all the way at the right
    }
    
    else if(directionInvader == 0){
     ++lineInvader;                      //move invader left
    }
    else if(directionInvader == 1){
      --lineInvader;                     //move invader right
    }
  }

//HITTING  
  if(columnLaser == columnInvader && lineLaser == lineInvader && columnLaser != 19){
  lcd.setCursor(columnLaser, lineInvader);
  lcd.write(3);                            //write poof
  columnLaser = 19;                        //reset laser
  columnInvader = 0;
  lineInvader = 0;                         //reset invader
  ++level;                                 //levelup
  ++number;                                //change invader
  if(number == 3) {
   number = 0;                             //reset number
  }
  delay(250);                              //short winners break
  }

//GAME OVER  
  if(columnInvader == 19){
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("Game Over");
   lcd.setCursor(0,2);
   lcd.print("Score: ");
   lcd.print(level);                      //game over screen with score
   delay(5000);                           //wait 5 sec before restarting
   
   columnInvader = 0;
   lineInvader = 0;
   counter = -1;
   columnLaser = 19;
   level = 0;
   difficulty = 3;                        //reset eveything
  }
  
//INCREASING DIFFICULTY
  if(level == 10 && difficulty == 3){
   --difficulty;                          //at level 10 the invaders will move faster
   counter = 0;                           //reset counter
  }

//COUNTING
 ++counter;                               //counter for animation
}
/*
Made by Thijs van Beers
Latest version: 27-5-2014
*/
