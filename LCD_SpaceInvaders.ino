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
const int invader[] = {4, 5, 6};
int number = 0;

//COLUMNS & LINES
int columnInvader = 0;
int lineInvader = 0;
int lineCanon = 0;
int columnLaser = 15;
int lineLaser = 0;

//OTHER INTS
int counter = 0;
int level = 0;
int difficulty = 3;

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
 lcd.begin(16, 2); 
}

void loop() {
  
//GOING LEFT/RIGHT
  int leftValue = analogRead(left);
  int rightValue = analogRead(right);
  if(leftValue > 511 && rightValue < 511){
    lineCanon = 0;
  }
  if(leftValue < 511 && rightValue > 511){
    lineCanon = 1;
  }
  
//SHOOTING  
  if(leftValue > 511 && rightValue > 511 && columnLaser == 15){
   lineLaser = lineCanon; 
  }
  if((leftValue > 511 && rightValue > 511) || columnLaser != 15){
    --columnLaser;
    lcd.setCursor(columnLaser, lineLaser);
    lcd.write(2);
  }
  if(columnLaser == 0){
   columnLaser = 15; 
  }
  
//ANIMATION  
  lcd.setCursor(15, lineCanon);
  lcd.write(1);

  lcd.setCursor(columnInvader, lineInvader);
  lcd.write(invader[number]);
  
  delay(200);
  lcd.clear();
  
  if(counter == difficulty){
    counter = 0;
    ++columnInvader;
    if(lineInvader == 0){
     lineInvader = 1; 
    }
    else if(lineInvader == 1){
     lineInvader = 0;
    }
  }

//HITTING  
  if(columnLaser == columnInvader && lineLaser == lineInvader && columnLaser != 15){
  lcd.setCursor(columnLaser, lineInvader);
  lcd.write(3);
  columnLaser = 15;
  columnInvader = 0;
  lineInvader = 0;
  ++level;
  ++number;
  if(number == 3) {
   number = 0; 
  }
  delay(250);  
  }

//GAME OVER  
  if(columnInvader == 15){
   for(int gameover = 0; gameover != 5; ++gameover){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game Over");
    lcd.setCursor(0,1);
    lcd.print("Score: ");
    lcd.print(level);
    delay(1000);
   }
  columnInvader = 0;
  lineInvader = 0;
  counter = -1;
  columnLaser = 15;
  level = 0;
  difficulty = 3; 
  }
  
//INCREASING DIFFICULTY
  if(level == 10 && difficulty == 3){
   --difficulty; 
   counter = 0;
  }

//COUNTING
 ++counter; 
}


