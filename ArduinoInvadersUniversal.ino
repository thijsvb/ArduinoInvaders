//EEPROM
#include <EEPROM.h>

//LCD SETUP
#include <LiquidCrystal.h>
const int lcdColumns = 20; //adjust to your own lcd
const int lcdLines = 4;    //adjust to your own lcd

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
int columnLaser = lcdColumns - 1; //the laser begins at the last column
int lineLaser = 0;

//OTHER INTS
int counter = 0;                  //count how many times the void loop has been completed
int level = 0;                    //how many invaders you have hit
int highscore = EEPROM.read(0);   //the highscore (stored on EEPROM so it is saved)
int difficulty = 3;               //the lower this number, the faster the invaders will go

//CONTROLS
const int left = A5;
const int right = A4;
const int shoot = A3;
const int stick = A5;             //you can have either a (left) button or a stick/potmeter connected to A5, you will not need both
int mode = 0;                     //adjust to your preferences (see below)
/*
0 = left button, right button, both pressed to shoot
1 = left button, right button, shoot button
2 = stick/potmeter for left/right, shoot button
*/

void setup() {
  
//CHARACTERS  
 lcd.createChar(1, canon);
 lcd.createChar(2, laser);
 lcd.createChar(3, poof);
 lcd.createChar(4, invaderZero);
 lcd.createChar(5, invaderOne);
 lcd.createChar(6, invaderTwo);
 
//STARTING LCD
 lcd.begin(lcdColumns, lcdLines);
 Serial.begin(9600); 
}

void loop() {
//INPUT VALUES
  int leftValue = analogRead(left);
  int rightValue = analogRead(right);
  int stickValue = analogRead(stick);
  int shootValue = analogRead(shoot);  

//GOING LEFT/RIGHT
 switch (mode) {
  case 0:
  case 1:  
  if(leftValue > 511 && rightValue < 511 && lineCanon != 0){            //left (left button pressed)
    --lineCanon;
  }
  if(leftValue < 511 && rightValue > 511 && lineCanon != lcdLines - 1){ //right (right button pressed)
    ++lineCanon;
  }
  break;
  
  case 2:
  lineCanon = map(stickValue, 0, 1023, 0, lcdLines - 1);                //change the value of the potmeter/stick to a line
  break;
 }
  
//SHOOTING 
 switch (mode) {
  case 0:
  if(leftValue > 511 && rightValue > 511 && columnLaser == lcdColumns - 1){ //shooting (both buttons pressed)
   lineLaser = lineCanon;                 //set laser on the correct line
   --columnLaser;
   lcd.setCursor(columnLaser, lineLaser);
   lcd.write(2);                          //move and write laser
  }
  
  break;
  case 1:
  case 2:
  if(shootValue > 511 && columnLaser == lcdColumns - 1){ //shooting (shoot button pressed)
   lineLaser = lineCanon;                 //set laser on the correct line
   --columnLaser;
   lcd.setCursor(columnLaser, lineLaser);
   lcd.write(2);                          //move and write laser
  }
  break;
  
 }
 if(columnLaser != lcdColumns - 1){
    --columnLaser;
    lcd.setCursor(columnLaser, lineLaser);
    lcd.write(2);                         //move and write laser
  }
 
 if(columnLaser == 0){
   columnLaser = lcdColumns - 1;          //reset laser
  }
  
//ANIMATION  
  lcd.setCursor(lcdColumns - 1, lineCanon);
  lcd.write(1);                          //write canon

  lcd.setCursor(columnInvader, lineInvader);
  lcd.write(invader[number]);            //write invader
  
  delay(200);
  lcd.clear();                           //delay & clear
  
  if(counter == difficulty){
    counter = 0;                         //reset counter
    
    if(directionInvader == 0 && lineInvader == lcdLines - 1){
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
  if(columnLaser == columnInvader && lineLaser == lineInvader && columnLaser != lcdColumns - 1){
  lcd.setCursor(columnLaser, lineInvader);
  lcd.write(3);                            //write poof
  columnLaser = lcdColumns - 1;            //reset laser
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
  if(columnInvader == lcdColumns - 1){
   if(level > highscore){
     highscore = level;
     EEPROM.write(0, highscore);
   }
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Game Over");
   lcd.setCursor(0,1);
   lcd.print("Score: ");
   lcd.print(level);                      
   lcd.setCursor(0,2);
   lcd.print("Highscore: ");
   lcd.print(highscore);                  //game over screen with score and highscore (highscore will not show up on a lcd with < 3 lines)
   delay(5000);                           //wait 5 sec before restarting
  
   columnInvader = 0;
   lineInvader = 0;
   counter = -1;
   columnLaser = lcdColumns - 1;
   level = 0;
   difficulty = 3;                       //reset everything
  }
  
//INCREASING DIFFICULTY
  if(level == 10 && difficulty == 3){
   --difficulty;                        //at level 10 the invaders will move faster
   counter = 0;                         //reset counter
  }

//COUNTING
 ++counter;                             //counter for animation
}
/*
Made by Thijs van Beers
Latest version: 29-5-2014
*/
