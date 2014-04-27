#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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

byte invader[8] = {
  0b00110,
  0b01000,
  0b11111,
  0b01110,
  0b01110,
  0b11111,
  0b01000,
  0b00110
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

int columnInvader = 0;
int lineInvader = 0;
int lineCanon = 0;
int columnLaser = 15;
int lineLaser = 0;
int counter = 0;
int level = 0;
int difficulty = 3;
const int left = A5;
const int right = A4;

void setup() {
 lcd.createChar(1, canon);
 lcd.createChar(2, laser);
 lcd.createChar(3, invader);
 lcd.createChar(4, poof);

 lcd.begin(16, 2);

 Serial.begin(9600); 
}

void loop() {
  int leftValue = analogRead(left);
  int rightValue = analogRead(right);
  if(leftValue > 511 && rightValue < 511){
    lineCanon = 0;
  }
  if(leftValue < 511 && rightValue > 511){
    lineCanon = 1;
  }
  if(leftValue > 511 && rightValue > 511 && columnLaser == 15){
   lineLaser = lineCanon; 
  }
  if((leftValue > 511 && rightValue > 511) || columnLaser != 15){
    --columnLaser;
    lcd.setCursor(columnLaser, lineLaser);
    lcd.write(2);
  }
  
  lcd.setCursor(15, lineCanon);
  lcd.write(1);

  lcd.setCursor(columnInvader, lineInvader);
  lcd.write(3);
  
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
  
  if(columnLaser == 0){
   columnLaser = 15; 
  }
  
  if(columnLaser == columnInvader && lineLaser == lineInvader && columnLaser != 15){
  lcd.setCursor(columnLaser, lineInvader);
  lcd.write(4);
  columnLaser = 15;
  columnInvader = 0;
  lineInvader = 0;
  ++level;
  delay(250);  
  }
  
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
  
  if(level == 10 && difficulty == 3 || level == 15 && difficulty == 2){
   --difficulty; 
   counter = 0;
  }
 ++counter; 
}


