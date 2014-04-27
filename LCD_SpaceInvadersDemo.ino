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
int columnLaser = 14;
int lineLaser = 0;

void setup() {
 lcd.createChar(1, canon);
 lcd.createChar(2, laser);
 lcd.createChar(3, invader);
 lcd.createChar(4, poof);

 lcd.begin(16, 2); 
}

void loop() {
  
  lcd.setCursor(15, lineCanon);
  lcd.write(1);
  
  lcd.setCursor(columnLaser, lineLaser);
  lcd.write(2);
  
  lcd.setCursor(columnInvader, lineInvader);
  lcd.write(3);
  
  if(columnInvader == 7 && lineInvader == 0){
   columnInvader = -1;
   lineInvader++; 
  }
  if(columnLaser == 7 && lineLaser == 0){
   columnLaser = 15;
   lineLaser++;
   lineCanon++; 
  }
  if(columnInvader == 7 && lineInvader == 1){
   columnInvader = -1;
   lineInvader--; 
  }
  if(columnLaser == 7 && lineLaser ==1){
   columnLaser = 15;
   lineLaser--;
   lineCanon--; 
  }
  
  columnInvader++;
  columnLaser--;
  delay(250);
  lcd.clear();
  
  if(columnLaser == 14 && columnInvader == 0 && lineCanon == 0){
   lcd.setCursor(7,1);
   lcd.write(4); 
  }
  
  if(columnLaser == 14 && columnInvader == 0 && lineCanon == 1){
   lcd.setCursor(7,0);
   lcd.write(4); 
  }
  
}
