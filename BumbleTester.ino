#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo victor1, spark1, victor2, spark2;
const int speed1 = A0, speed2 = A1, forward1 = A2, backward1 = A3, forward2 = 3, backward2 = 4, both = 2;
int direction1 = 0, direction2 = 0, power1 = 0, power2 = 0, lastPower1 = 1, lastPower2 = 1, lastDirection1 = 1, lastDirection2 = 1, powerPercentage1 = 1, powerPercentage2 = 1, useBoth = 0;

 LiquidCrystal_I2C lcd(0x27, 16, 2);

void setUpScreen(){
  lcd.clear();
  lcd.print("Power 1: ");
  lcd.setCursor(15, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Power 2: ");
  lcd.setCursor(15, 1);
  lcd.print("%");
}

void readSpeeds(){
  power1 = analogRead(speed1);
  power2 = analogRead(speed2);
  if(useBoth == 1){
    power2 = power1;
  }
  powerPercentage1 = map(power1, 0, 1023, 0, 100);
  powerPercentage2 = map(power2, 0, 1023, 0, 100);
}

void readToggles(){
  // reading toggle 1 direction
  if(!digitalRead(forward1)){
    direction1 = 1;
  }else if(!digitalRead(backward1)){
    direction1 = 2;
  }else{
    direction1 = 0;
  }

  // reading toggle 2 direction
  if(!digitalRead(forward2)){
    direction2 = 1;
  }else if(!digitalRead(backward2)){
    direction2 = 2;
  }else{
    direction2 = 0;
  }
  useBoth = !digitalRead(both);
}

void writePowerToControllers(){
  // getting output for controller 1
  int maxOut = 1500;
  if(direction1 == 1){
    maxOut = 2000;
  }else if(direction1 == 2){
    maxOut = 1000;
  }
  int output1 = map(power1, 0, 1023, 1500, maxOut);

  // getting output for controller 2
  maxOut = 1500;
  if(direction2 == 1){
    maxOut = 2000;
  }else if(direction2 == 2){
    maxOut = 1000;
  }
  int output2 = map(power2, 0, 1023, 1500, maxOut);

  // writing to controllers
  victor1.writeMicroseconds(output1);
  spark1.writeMicroseconds(output1);
  victor2.writeMicroseconds(output2);
  spark2.writeMicroseconds(output2);
}

void showOutputPower(){
  //clear screen
  for(int x = 12; x < 15; x++){
    lcd.setCursor(x, 0);
    lcd.print(" ");
    lcd.setCursor(x, 1);
    lcd.print(" ");
  }

  // print motor 1 power
  if(powerPercentage1 == 100){
    lcd.setCursor(12, 0);
  }else{
    lcd.setCursor(13, 0);
  }
  lcd.print(powerPercentage1);

  
  // print motor 2 power
  if(powerPercentage2 == 100){
    lcd.setCursor(12, 1);
  }else{
    lcd.setCursor(13, 1);
  }
  lcd.print(powerPercentage2);
}

void setup() {
  pinMode(speed1, INPUT);
  pinMode(speed2, INPUT);
  pinMode(forward1, INPUT_PULLUP);
  pinMode(backward1, INPUT_PULLUP);
  pinMode(forward2, INPUT_PULLUP);
  pinMode(backward2, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(12,OUTPUT);

  digitalWrite(5, 0);
  digitalWrite(12,0);

  victor1.attach(8);
  spark1.attach(9);
  victor2.attach(6);
  spark2.attach(7);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  lcd.setCursor(0, 1);
  lcd.print("BumbleTester");
  delay(2000);
  setUpScreen();
}

void loop() {
  readToggles();
  readSpeeds();
  if(powerPercentage1 != lastPower1 || powerPercentage2 != lastPower2 || direction1 != lastDirection1 || direction2 != lastDirection2){
    writePowerToControllers();
    showOutputPower();
  }
  lastPower1 = powerPercentage1;
  lastPower2 = powerPercentage2;
  lastDirection1 = direction1;
  lastDirection2 = direction2;
  delay(20);
}
